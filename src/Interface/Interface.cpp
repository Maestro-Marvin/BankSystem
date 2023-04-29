#include "Interface.hpp"

Interface::Interface() {
  data_storage_ = DataStorage();
  timer_ = Timer();
}

void Interface::clear_information() {
  for (int bank_id = 1; bank_id <= 2; ++bank_id) {
    banks_[bank_id].clear_information();
  }
}

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
  if (mode == "sign up" && find_user(user, password)) {
    return std::make_pair(std::vector<User>(), 0);
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
                                              std::vector<User> users,
                                              Timer& timer) {
  std::ofstream file;
  file.open("../src/Interface/info.txt", std::ios::trunc);
  for (auto& user : users) {
    file << user.username << '\n';
    file << user.password << '\n';
    for (auto& line : user.client->get_info()) {
      file << line << '\n';
    }
    for (int bank_id = 1; bank_id <= 2; ++bank_id) {
      auto data = banks[bank_id].get_accounts_belong_to(user.client,
                                                        timer.curr_period());
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
  print_message("");
  while (option < 1 || option > 2) {
    print_message("Incorrect option, try again");
    std::cin >> option;
  }
  std::string user, password;
  if (option == 1) {
    print_message("Write user name then write password");
    std::cin >> user >> password;
    print_message("");
    auto ans =
        data_storage_.restore_information(banks_, user, password, "sign in");
    while (ans.second == -1) {
      print_message("Such user doesn't exist, try again");
      std::cin >> user >> password;
      print_message("");
      ans =
          data_storage_.restore_information(banks_, user, password, "sign in");
    }
    users = ans.first;
    client = users[ans.second].client;
  } else {
    print_message("Create user name then create password");
    std::cin >> user >> password;
    print_message("");
    auto ans =
        data_storage_.restore_information(banks_, user, password, "sign up");
    while (ans.second == 0) {
      print_message("Such user already exists, try again");
      std::cin >> user >> password;
      print_message("");
      ans =
          data_storage_.restore_information(banks_, user, password, "sign up");
    }
    users = ans.first;
    client = client->build_client();
    users.push_back({user, password, client});
  }
}

void Interface::list_of_accounts(Client* const& person) {
  for (int bank_id = 1; bank_id <= 2; ++bank_id) {
    auto data =
        banks_[bank_id].get_accounts_belong_to(person, timer_.curr_period());
    if (!data.empty()) {
      print_message(bank_names_[bank_id]);
    }
    int curr_line = 0;
    while (curr_line < data.size()) {
      print_message("Account id : " + data[curr_line++]);
      std::string type = data[curr_line];
      print_message("Account type : " + data[curr_line++]);
      print_message("Account balance : " + data[curr_line++]);
      if (type == "Deposit") {
        print_message("Remaining deposit time : " + data[curr_line++]);
      }
    }
  }
}

Client* Interface::create_client() {
  Client* client;
  client = client->build_client();
  return client;
}

void Interface::set_adress(Client*& client) {
  print_message("Write your adress");
  std::string adress;
  std::cin >> adress;
  client->set_adress(adress);
}

void Interface::set_passport(Client*& client) {
  print_message("Write your passport");
  std::string passport;
  std::cin >> passport;
  client->set_passport(passport);
}

Bank* Interface::create_bank() {
  print_message("Choose bank:\n1.tinkoff\n2.sberbank");
  int num;
  std::cin >> num;
  print_message("");
  while (num < 1 || num > 2) {
    print_message("Incorrect bank, try again");
    std::cin >> num;
  }
  return &banks_[num];
}

int Interface::create_account(Client*& person) {
  Bank* bank = create_bank();
  print_message("Choose account type:\n1.debet\n2.deposit\n3.credit");
  int num;
  std::cin >> num;
  print_message("");
  while (num < 1 || num > 3) {
    print_message("Incorrect account type, try again");
    std::cin >> num;
  }
  std::map<int, std::string> accounts = {
      {1, "Debet"}, {2, "Deposit"}, {3, "Credit"}};
  std::string type = accounts[num];
  int id = bank->open_account(person, type);
  return id;
}

void Interface::close_account(Client* const& person) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  print_message("");
  if (bank->is_belong(id, person)) {
    bank->close_account(id);
  } else {
    print_message("It's not your account");
  }
}

void Interface::balance(Client* const& person) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  print_message("");
  if (!bank->exist(id)) {
    print_message("There isn't such account");
    return;
  }
  if (!bank->is_belong(id, person)) {
    print_message("It's not your account");
    return;
  }
  int bal = bank->balance(id);
  print_message("Your balance is " + std::to_string(bal));
}

void Interface::Timer::start_timer() { start_ = time(&start_); }

int Interface::Timer::curr_period() {
  time_t curr;
  time(&curr);
  int period = static_cast<double>(curr - start_) / day_;
  return period;
}

bool Interface::compare_period(Bank*& bank, int id) {
  if (bank->get_type(id) != "Deposit") {
    return true;
  }
  int period = timer_.curr_period();
  if (period >= bank->get_period(id)) {
    return true;
  }
  return false;
}

void Interface::withdraw(Client* const& person) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  print_message("");
  if (!bank->exist(id)) {
    print_message("There isn't such account");
    return;
  }
  if (!bank->is_belong(id, person)) {
    print_message("It's not your account");
    return;
  }
  if (!compare_period(bank, id)) {
    print_message("Deposit period isn't over");
    return;
  }
  print_message("Write sum");
  int sum;
  std::cin >> sum;
  bank->withdraw(id, sum);
}

void Interface::refill(Client* const& person) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  print_message("");
  if (!bank->exist(id)) {
    print_message("There isn't such account");
    return;
  }
  if (!bank->is_belong(id, person)) {
    print_message("It's not your account");
    return;
  }
  print_message("Write sum");
  int sum;
  std::cin >> sum;
  bank->refill(id, sum);
}

void Interface::transaction(Client* const& sender) {
  Bank* bank1 = create_bank();
  print_message("Write sender account id");
  int id1;
  std::cin >> id1;
  print_message("");
  if (!bank1->exist(id1)) {
    print_message("There isn't such account");
    return;
  }
  if (!bank1->is_belong(id1, sender)) {
    print_message("It's not your account");
    return;
  }
  if (!compare_period(bank1, id1)) {
    print_message("Deposit period isn't over");
    return;
  }
  Bank* bank2 = create_bank();
  print_message("Write reciever account id");
  int id2;
  std::cin >> id2;
  print_message("");
  if (!bank2->exist(id2)) {
    print_message("There isn't such account");
    return;
  }
  if (bank1 == bank2 && id1 == id2) {
    print_message("It's the same account");
    return;
  }
  print_message("Write sum");
  int sum;
  std::cin >> sum;
  if (bank1->withdraw(id1, sum) != 0) {
    bank2->refill(id2, sum);
  }
}

void Interface::cancel_last_operation(Client* const& person) {
  Bank* bank = create_bank();
  print_message("Write your account id");
  int id;
  std::cin >> id;
  print_message("");
  if (!bank->exist(id)) {
    print_message("There isn't such account");
    return;
  }
  if (!bank->is_belong(id, person)) {
    print_message("It's not your account");
    return;
  }
  bank->cancel_last_operation(id);
}