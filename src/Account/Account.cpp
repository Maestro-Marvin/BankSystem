#include "Account.hpp"

int Account::balance() const { return balance_; }

bool Account::is_belong(Client* const& person) const {
  return person == person_;
}

bool Account::is_same_phone(const std::string& phone) const {
  return person_->get_phone() == phone;
}

void Account::cancel_last_operation() { balance_ = prev_balance_; }
