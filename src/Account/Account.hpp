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

  int balance() const;

  bool is_belong(Client* const& person) const;

  bool is_same_phone(const std::string& phone) const;

  virtual std::string type() = 0;

  virtual int withdraw(int) = 0;  // return 0 if the operation was unsuccessful

  virtual void refill(int) = 0;

  void cancel_last_operation();

  virtual ~Account() = default;
};

class DebetAccount : public Account {
 public:
  std::string type() override;

  DebetAccount(Client* person);

  int withdraw(int sum) override;

  void refill(int sum) override;
};

class DepositAccount : public Account {
  int period_;

 public:
  std::string type() override;

  DepositAccount(Client* person, int period);

  void decrease_period(int period);

  void set_period(int period);

  int get_period() const;

  int withdraw(int sum) override;

  void refill(int sum) override;
};

class CreditAccount : public Account {
  int limit_;
  int percentage_;

 public:
  std::string type() override;

  CreditAccount(Client* person, int limit, int percentage);

  int withdraw(int sum) override;

  void refill(int sum) override;
};
