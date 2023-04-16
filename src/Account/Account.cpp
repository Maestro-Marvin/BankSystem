#include "Account.hpp"

#include <iostream>
#include <string>

#include "Client.hpp"

int Account::Balance() const { return balance_; }

Client* Account::GetClient() { return person_; }

void Account::CancelLastOperation() { balance_ = prev_balance_; }

DebetAccount::DebetAccount(Client* person) : Account() { person_ = person; }

std::string DebetAccount::Type() { return "Debet"; }

int DebetAccount::Withdraw(int sum) {
  if (!person_->IsReliable()) {
    std::cout << "Withdraw operation declined, set passport or adress\n";
    return 0;
  }
  if (balance_ < sum) {
    std::cout << "Insufficient funds\n";
    return 0;
  }
  prev_balance_ = balance_;
  balance_ -= sum;
  return 1;
}

void DebetAccount::Refill(int sum) {
  prev_balance_ = balance_;
  balance_ += sum;
}

DepositAccount::DepositAccount(Client* person, int period) : period_(period) {
  person_ = person;
}

void DepositAccount::DecreasePeriod() { period_--; }

std::string DepositAccount::Type() { return "Deposit"; }

int DepositAccount::Withdraw(int sum) {
  if (period_ > 0) {
    std::cout << "The deposit period isn't over\n";
    return 0;
  }
  if (!person_->IsReliable()) {
    std::cout << "Withdraw operation declined, set passport or adress\n";
    return 0;
  }
  if (balance_ < sum) {
    std::cout << "Insufficient funds\n";
    return 0;
  }
  prev_balance_ = balance_;
  balance_ -= sum;
  return 1;
}

void DepositAccount::Refill(int sum) {
  prev_balance_ = balance_;
  balance_ += sum;
}

CreditAccount::CreditAccount(Client* person, int limit, int percentage)
    : limit_(limit), percentage_(percentage) {
  person_ = person;
}

std::string CreditAccount::Type() { return "Credit"; }

int CreditAccount::Withdraw(int sum) {
  if (!person_->IsReliable()) {
    std::cout << "Withdraw operation declined, set passport or adress\n";
    return 0;
  }
  int sum_to_withdraw =
      (int)((double)(sum) * (1 + (double)(percentage_) / 100));
  if (balance_ < sum_to_withdraw) {
    std::cout << "Insufficient funds\n";
    return 0;
  }
  prev_balance_ = balance_;
  balance_ -= sum_to_withdraw;
  return 1;
}

void CreditAccount::Refill(int sum) {
  prev_balance_ = balance_;
  balance_ += (int)((double)(sum) * (1 - (double)(percentage_) / 100));
}
