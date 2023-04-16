#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Account.hpp"

class Bank {
  std::string name_;
  int last_id = 1;
  std::map<int, Account*> accounts_;

  // for credit account
  const int limit_;
  const int percentage_;

 public:
  Bank();

  Bank(std::string name, int limit, int percentage);

  std::vector<std::string> GetAccounts(Client* client);

  bool Exist(int id) const;

  int OpenAccount(Client* person, std::string type, int certain_id = -1,
                  bool is_mute = true);

  void CloseAccount(int id);

  int Balance(int id);

  int Withdraw(int id, int sum);

  void Refill(int id, int sum);

  void Transaction(int id_sender, int id_receiver, int sum);

  void CancelLastOperation(int id);

  void DecreasePeriod(int id);

  ~Bank();
};
