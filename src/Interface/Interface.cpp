#include "Interface.hpp"

bool Interface::DataStorage::find_user(std::string user, std::string password) {
  std::ifstream file;
  file.open("../src/Interface/info.txt");
  std::string line;
  while (std::getline(file, line)) {
    if (line == user) {
      if (std::getline(file, line) && line == password) {
        file.close();
        return true;
      }
    }
  }
  file.close();
  return false;
}

Client* Interface::DataStorage::restore_client(std::ifstream& file) {
  std::string first_name;
  std::getline(file, first_name);
  std::string second_name;
  std::getline(file, second_name);
  std::string adress;
  std::getline(file, adress);
  std::string passport;
  std::getline(file, passport);
  return new Client(first_name, second_name, adress, passport);
}

void Interface::DataStorage::restore_accounts(std::ifstream& file,
                                              std::map<int, Bank>& banks,
                                              Client* client) {
  std::string line;
  while (file >> line && line != "###") {
    int bank_id = std::stoi(line);
    Bank& bank = banks[bank_id];
    int account_id;
    file >> account_id;
    std::string type;
    file >> type;
    int sum;
    file >> sum;
    bank.open_account(client, type, account_id, true);
    if (type == "Deposit") {
      int period;
      file >> period;
      bank.set_period(account_id, period);
    }
    bank.refill(account_id, sum);
  }
}

std::pair<std::vector<User>, int> Interface::DataStorage::restore_information(
    std::map<int, Bank>& banks, std::string user, std::string password,
    std::string mode) {
  if (mode == "sign in" && !find_user(user, password)) {
    return std::make_pair(std::vector<User>(), -1);
  }
  std::ifstream file;
  file.open("../src/Interface/info.txt");
  std::vector<User> ans;
  int idx = -1;
  std::string line;
  while (std::getline(file, line) && !line.empty()) {
    std::string userr;
    std::string passwordd;
    userr = line;
    std::getline(file, line);
    passwordd = line;
    Client* client = restore_client(file);
    restore_accounts(file, banks, client);
    if (userr == user && passwordd == password) {
      idx = ans.size();
    }
    ans.push_back({userr, passwordd, client});
    std::getline(file, line);
  }
  file.close();
  return std::make_pair(ans, idx);
}

void Interface::DataStorage::save_information(std::map<int, Bank>& banks,
                                              std::vector<User> users) {
  std::ofstream file;
  file.open("../src/Interface/info.txt", std::ios::trunc);
  for (auto& user : users) {
    file << user.username << '\n';
    file << user.password << '\n';
    for (auto& line : user.client->get_info()) {
      file << line << '\n';
    }
    for (int bank_id = 1; bank_id <= 2; ++bank_id) {
      auto data = banks[bank_id].get_accounts(user.client);
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

void Interface::user_register(Client*& client, std::vector<User>& users) {
  print_message("Hello, welcome to account manager");
  print_message("1.Sign in\n2.Sign up");
  int option;
  std::cin >> option;
  while (option < 1 || option > 2) {
    print_message("Incorrect option, try again");
    std::cin >> option;
  }
  std::string user, password;
  if (option == 1) {
    print_message("Write user name then write password");
    std::cin >> user >> password;
    auto ans =
        data_storage_.restore_information(banks_, user, password, "sign in");
    while (ans.second == -1) {
      print_message("Such user doesn't exist, try again");
      std::cin >> user >> password;
      ans =
          data_storage_.restore_information(banks_, user, password, "sign in");
    }
    users = ans.first;
    client = users[ans.second].client;
  } else {
    print_message("Create user name then create password");
    std::cin >> user >> password;
    users = data_storage_.restore_information(banks_, user, password, "sign up")
                .first;
    client = client->build_client();
    users.push_back({user, password, client});
  }
}

Client* Interface::create_client() {
  Client* client;
  client = client->build_client();
  return client;
}

void Interface::set_adress(Client* client) {
  print_message("Write your adress");
  std::string adress;
  std::cin >> adress;
  client->set_adress(adress);
}

void Interface::set_passport(Client* client) {
  print_message("Write your passport");
  std::string passport;
  std::cin >> passport;
  client->set_passport(passport);
}

Bank* Interface::create_bank() {
  print_message("Choose bank:\n1.tinkoff\n2.sberbank");
  int num;
  std::cin >> num;
  while (num < 1 || num > 2) {
    print_message("Incorrect bank, try again");
    std::cin >> num;
  }
  return &banks_[num];
}

int Interface::create_account(Client* client) {
  Bank* bank = create_bank();
  print_message("Choose account type:\n1.debet\n2.deposit\n3.credit");
  int num;
  std::cin >> num;
  while (num < 1 || num > 3) {
    print_message("Incorrect account type, try again");
    std::cin >> num;
  }
  std::map<int, std::string> accounts = {
      {1, "Debet"}, {2, "Deposit"}, {3, "Credit"}};
  std::string type = accounts[num];
  int id = bank->open_account(client, type);
  return id;
}

void Interface::close_account(Client* client) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  bank->close_account(id);
}

void Interface::balance() {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  int bal = bank->balance(id);
  std::cout << "Your balance is " << bal << "\n";
}

void Interface::withdraw() {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  if (bank->exist(id)) {
    print_message("Write sum");
    int sum;
    std::cin >> sum;
    bank->withdraw(id, sum);
  }
}

void Interface::refill() {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  if (bank->exist(id)) {
    print_message("Write sum");
    int sum;
    std::cin >> sum;
    bank->refill(id, sum);
  }
}

void Interface::transaction() {
  Bank* bank1 = create_bank();
  print_message("Write sender account id");
  int id1;
  std::cin >> id1;
  if (!bank1->exist(id1)) {
    return;
  }
  Bank* bank2 = create_bank();
  print_message("Write reciever account id");
  int id2;
  std::cin >> id2;
  if (!bank2->exist(id2)) {
    return;
  }
  print_message("Write sum");
  int sum;
  std::cin >> sum;
  if (bank1->withdraw(id1, sum) != 0) {
    bank2->refill(id2, sum);
  }
}

void Interface::cancel_last_operation() {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  bank->cancel_last_operation(id);
}

void Interface::decrease_period() {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  bank->decrease_period(id);
}