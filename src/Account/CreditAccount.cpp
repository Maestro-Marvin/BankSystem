#include "Account.hpp"

CreditAccount::CreditAccount(Client* person, int limit, int percentage)
    : limit_(limit), percentage_(percentage) {
  person_ = person;
}

std::string CreditAccount::type() { return "Credit"; }

int CreditAccount::withdraw(int sum) {
  if (!person_->is_reliable()) {
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

void CreditAccount::refill(int sum) {
  prev_balance_ = balance_;
  balance_ += (int)((double)(sum) * (1 - (double)(percentage_) / 100));
}