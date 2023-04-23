#include "Account.hpp"

DebetAccount::DebetAccount(Client* person) { person_ = person; }

std::string DebetAccount::type() { return "Debet"; }

int DebetAccount::withdraw(int sum) {
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

void DebetAccount::refill(int sum) {
  prev_balance_ = balance_;
  balance_ += sum;
}