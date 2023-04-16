#pragma once
#include <iostream>
#include <string>

#include "Client.hpp"

class Account {
 protected:
  int prev_balance_;
  int balance_ = 0;
  Client* person_;

 public:
  Account() = default;

  int Balance() const;

  Client* GetClient();

  virtual std::string Type() = 0;

  virtual int Withdraw(int) = 0;  // return 0 if the operation was unsuccessful

  virtual void Refill(int) = 0;

  void CancelLastOperation();

  virtual ~Account() = default;
};

class DebetAccount : public Account {
 public:
  std::string Type() override;

  DebetAccount(Client* person);

  int Withdraw(int sum) override;

  void Refill(int sum) override;
};

class DepositAccount : public Account {
  int period_;

 public:
  std::string Type() override;

  DepositAccount(Client* person, int period);

  void DecreasePeriod();

  int Withdraw(int sum) override;

  void Refill(int sum) override;
};

class CreditAccount : public Account {
  int limit_;
  int percentage_;

 public:
  std::string Type() override;

  CreditAccount(Client* person, int limit, int percentage);

  int Withdraw(int sum) override;

  void Refill(int sum) override;
};
