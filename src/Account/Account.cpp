#include "Account.hpp"

int Account::balance() const { return balance_; }

bool Account::is_belong(Client* const& person) const {
  return person == person_;
}

void Account::cancel_last_operation() { balance_ = prev_balance_; }
