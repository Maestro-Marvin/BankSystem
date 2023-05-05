#pragma once
#include <time.h>

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
  std::map<int, std::string> bank_names_ = {{1, "tinkoff"}, {2, "sberbank"}};

  std::map<int, Bank> banks_ = {{1, Bank("tinkfoff", 10000, 10)},
                                {2, Bank("sberbank", 15000, 15)}};

  Client* create_client();

  void clear_information();

  void set_phone(Client*& client);

  void set_adress(Client*& client);

  void set_passport(Client*& client);

  int get_account_id(Bank*& bank);

  Bank* create_bank();

  void list_of_accounts(Client* const& person);

  int create_account(Client*& person);

  void close_account(Client* const& person);

  void balance(Client* const& person);

  void withdraw(Client* const& person);

  void refill(Client* const& person);

  void transaction(Client* const& sender);

  void cancel_last_operation(Client* const& person);

  bool compare_period(Bank*& bank, int id);

  void decrease_period(Bank*& bank, int id);

  void user_register(Client*& client, std::vector<User>& users);

  struct Timer {
    double day_ = 15.0;  // in seconds

    time_t start_;

    void start_timer();

    int curr_period();
  };

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

    void save_information(std::map<int, Bank>& banks, std::vector<User> users,
                          Timer& timer);
  };

  virtual void print_message(std::string message) = 0;

  DataStorage data_storage_;

  Timer timer_;

 public:
  Interface();

  virtual void main() = 0;
};

class ConsoleInterface : public Interface {
  void print_message(std::string message) override;

 public:
  ConsoleInterface() = default;

  void main() override;
};
