#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Account.hpp"
#include "Bank.hpp"
#include "Client.hpp"

struct User {
  std::string username;
  std::string password;
  Client* client;
};

class Interface {
 protected:
  std::map<int, Bank> banks_ = {{1, Bank("tinkfoff", 10000, 10)},
                                {2, Bank("sberbank", 15000, 15)}};

  Client* create_client();

  void set_adress(Client* client);

  void set_passport(Client* client);

  Bank* create_bank();

  int create_account(Client* client);

  void close_account(Client* client);

  void balance();

  void withdraw();

  void refill();

  void transaction();

  void cancel_last_operation();

  void decrease_period();

  void user_register(Client*& client, std::vector<User>& users);

  class DataStorage {
   public:
    DataStorage() = default;

    bool find_user(std::string user, std::string password);

    Client* restore_client(std::ifstream& file);

    void restore_accounts(std::ifstream& file, std::map<int, Bank>& banks,
                          Client* client);

    std::pair<std::vector<User>, int> restore_information(
        std::map<int, Bank>& banks, std::string user, std::string password,
        std::string mode);
    void save_information(std::map<int, Bank>& banks, std::vector<User> users);
  };

  virtual void print_message(std::string message) = 0;

  DataStorage data_storage_;

 public:
  Interface() = default;

  virtual void main() = 0;
};

class ConsoleInterface : public Interface {
  void print_message(std::string message) override;

 public:
  ConsoleInterface() { data_storage_ = DataStorage(); }

  void main() override;
};