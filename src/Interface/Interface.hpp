#pragma once
#include <fstream>
#include <iostream>
#include <map>
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
  std::pair<std::vector<User>, int> RestoreInformation(std::string user,
                                                       std::string password);

  void SaveInformation(std::vector<User> users);

 public:
  Interface() = default;

  virtual void PrintMessage(std::string message) = 0;

  Client* CreateClient();

  void SetAdress(Client* client);

  void SetPassport(Client* client);

  std::map<int, Bank> banks_ = {{1, Bank("tinkfoff", 10000, 10)},
                                {2, Bank("sberbank", 15000, 15)}};

  Bank* CreateBank();

  int CreateAccount(Client* client);

  void CloseAccount(Client* client);

  void Balance();

  void Withdraw();

  void Refill();

  void Transaction();

  void CancelLastOperation();

  void DecreasePeriod();

  virtual void main() = 0;
};

class ConsoleInterface : public Interface {
  void PrintMessage(std::string message) override;

  void main() override;
};