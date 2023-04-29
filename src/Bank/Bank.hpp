#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Account.hpp"

class Bank {
  std::string name_;
  int last_id_ = 1;
  std::map<int, Account*> accounts_;

  // for credit account
  const int kLimit;
  const int kPercentage;

 public:
  Bank();

  Bank(std::string name, int limit, int percentage);

  std::vector<std::string> get_accounts_belong_to(Client* const& client,
                                                  int passed_period);

  bool exist(int id) const;

  bool is_belong(int id, Client* const& person);

  int open_account(Client*& person, std::string type, int certain_id = -1,
                   bool is_mute = false);

  void close_account(int id);

  void clear_information();

  int balance(int id);

  int withdraw(int id, int sum);

  void refill(int id, int sum);

  void transaction(int id_sender, int id_receiver, int sum);

  void cancel_last_operation(int id);

  void set_period(int id, int period);

  int get_period(int id);

  std::string get_type(int id);

  ~Bank();
};
