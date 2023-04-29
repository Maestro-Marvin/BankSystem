#include "Account.hpp"

DepositAccount::DepositAccount(Client* person, int period) : period_(period) {
  person_ = person;
}

void DepositAccount::decrease_period(int period) {
  period_ = std::max(0, period_ - period);
}

void DepositAccount::set_period(int period) { period_ = period; }

int DepositAccount::get_period() const { return period_; }

std::string DepositAccount::type() { return "Deposit"; }

int DepositAccount::withdraw(int sum) {
  if (!person_->is_reliable()) {
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

void DepositAccount::refill(int sum) {
  prev_balance_ = balance_;
  balance_ += sum;
}
