#include "Account.hpp"

int Account::balance() const { return balance_; }

Client* Account::get_client() { return person_; }

void Account::cancel_last_operation() { balance_ = prev_balance_; }
