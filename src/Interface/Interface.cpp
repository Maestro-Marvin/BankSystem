#include "Interface.hpp"

std::pair<std::vector<User>, int> Interface::RestoreInformation(
    std::string user, std::string password) {
  std::ifstream file;
  file.open("../src/Interface/info.txt");
  std::vector<User> ans;
  int idx = -1;
  std::string line;
  while (std::getline(file, line) && !line.empty()) {
    std::string us, pas;
    us = line;
    std::getline(file, line);
    pas = line;
    std::string first_name;
    std::getline(file, first_name);
    std::string second_name;
    std::getline(file, second_name);
    std::string adress;
    std::getline(file, adress);
    std::string passport;
    std::getline(file, passport);
    Client* client = new Client(first_name, second_name, adress, passport);
    while (file >> line && line != "###") {
      int bank_id = std::stoi(line);
      Bank& bank = banks_[bank_id];
      int account_id;
      file >> account_id;
      std::string type;
      file >> type;
      int sum;
      file >> sum;
      bank.OpenAccount(client, type, account_id, false);
      bank.Refill(account_id, sum);
    }
    if (us == user && pas == password) {
      idx = ans.size();
    }
    ans.push_back({us, pas, client});
    std::getline(file, line);
  }
  file.close();
  return std::make_pair(ans, idx);
}

void Interface::SaveInformation(std::vector<User> users) {
  std::ofstream file;
  file.open("../src/Interface/info.txt", std::ios::trunc);
  for (auto& user : users) {
    file << user.username << '\n';
    file << user.password << '\n';
    for (auto& line : user.client->GetInfo()) {
      file << line << '\n';
    }
    for (int bank_id = 1; bank_id <= 2; ++bank_id) {
      auto data = banks_[bank_id].GetAccounts(user.client);
      if (!data.empty()) {
        file << bank_id << '\n';
      }
      for (auto& line : data) {
        file << line << '\n';
      }
    }
    file << "###\n";
  }
  file.close();
}

Client* Interface::CreateClient() {
  Client* client = BuildClient();
  return client;
}

void Interface::SetAdress(Client* client) {
  PrintMessage("Write your adress");
  std::string adress;
  std::cin >> adress;
  client->SetAdress(adress);
}

void Interface::SetPassport(Client* client) {
  PrintMessage("Write your passport");
  std::string passport;
  std::cin >> passport;
  client->SetPassport(passport);
}

Bank* Interface::CreateBank() {
  PrintMessage("Choose bank: tinkoff or sberbank");
  int num;
  std::cin >> num;
  while (num < 1 || num > 2) {
    PrintMessage("Incorrect bank, try again");
    std::cin >> num;
  }
  return &banks_[num];
}

int Interface::CreateAccount(Client* client) {
  Bank* bank = CreateBank();
  PrintMessage("Choose account type: debet, deposit or credit");
  int num;
  std::cin >> num;
  while (num < 1 || num > 3) {
    PrintMessage("Incorrect account type, try again");
    std::cin >> num;
  }
  std::map<int, std::string> accounts = {
      {1, "Debet"}, {2, "Deposit"}, {3, "Credit"}};
  std::string type = accounts[num];
  int id = bank->OpenAccount(client, type);
  return id;
}

void Interface::CloseAccount(Client* client) {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  bank->CloseAccount(id);
}

void Interface::Balance() {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  int bal = bank->Balance(id);
  std::cout << "Your balance is " << bal << "\n";
}

void Interface::Withdraw() {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  PrintMessage("Write sum");
  int sum;
  std::cin >> sum;
  bank->Withdraw(id, sum);
}

void Interface::Refill() {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  PrintMessage("Write sum");
  int sum;
  std::cin >> sum;
  bank->Refill(id, sum);
}

void Interface::Transaction() {
  Bank* bank1 = CreateBank();
  PrintMessage("Write sender account id");
  int id1;
  std::cin >> id1;
  Bank* bank2 = CreateBank();
  PrintMessage("Write reciever account id");
  int id2;
  std::cin >> id2;
  PrintMessage("Write sum");
  int sum;
  std::cin >> sum;
  if (bank1->Exist(id1) && bank2->Exist(id2)) {
    if (bank1->Withdraw(id1, sum) != 0) {
      bank2->Refill(id2, sum);
    }
  }
}

void Interface::CancelLastOperation() {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  bank->CancelLastOperation(id);
}

void Interface::DecreasePeriod() {
  Bank* bank = CreateBank();
  PrintMessage("Write your account id");
  int id;
  std::cin >> id;
  bank->DecreasePeriod(id);
}

void ConsoleInterface::PrintMessage(std::string message) {
  std::cout << message << "\n";
}

void ConsoleInterface::main() {
  PrintMessage("Hello, welcome to account manager");
  PrintMessage("sign in or sign up");
  Client* client;
  std::vector<User> users;
  int option;
  std::cin >> option;
  while (option < 1 || option > 2) {
    PrintMessage("Incorrect option, try again");
    std::cin >> option;
  }
  std::string user, password;
  if (option == 1) {
    PrintMessage("Write user name then write password");
    std::cin >> user >> password;
    auto ans = RestoreInformation(user, password);
    while (ans.second == -1) {
      PrintMessage("Incorrect data, try again");
      std::cin >> user >> password;
    }
    users = ans.first;
    client = users[ans.second].client;
  } else {
    PrintMessage("Create user name then create password");
    std::cin >> user >> password;
    users = RestoreInformation(user, password).first;
    client = BuildClient();
    users.push_back({user, password, client});
  }
  bool run = true;
  while (run) {
    PrintMessage("Choose an option");
    PrintMessage(
        "create bank account, show balance, withdraw, refill, transfer money, "
        "cancel last operation, decrease period, set adress, set passport, "
        "exit");
    int option;
    std::cin >> option;
    while (option < 1 || option > 10) {
      PrintMessage("Incorrect option, try again");
      std::cin >> option;
    }
    if (option == 1) {
      CreateAccount(client);
    } else if (option == 2) {
      Balance();
    } else if (option == 3) {
      Withdraw();
    } else if (option == 4) {
      Refill();
    } else if (option == 5) {
      Transaction();
    } else if (option == 6) {
      CancelLastOperation();
    } else if (option == 7) {
      DecreasePeriod();
    } else if (option == 8) {
      SetAdress(client);
    } else if (option == 9) {
      SetPassport(client);
    } else if (option == 10) {
      SaveInformation(users);
      run = false;
    }
  }
}