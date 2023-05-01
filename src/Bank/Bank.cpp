#include "Bank.hpp"

std::vector<std::string> Bank::get_accounts_belong_to(Client* const& client,
                                                      int passed_period) {
  std::vector<std::string> ans;
  for (auto pair : accounts_) {
    int id = pair.first;
    Account* account = pair.second;
    if (account->is_belong(client)) {
      ans.push_back(std::to_string(id));
      ans.push_back(account->type());
      ans.push_back(std::to_string(balance(id)));
      if (account->type() == "Deposit") {
        int period = static_cast<DepositAccount*>(account)->get_period();
        period = std::max(0, period - passed_period);
        ans.push_back(std::to_string(period));
      }
    }
  }
  return ans;
}

bool Bank::exist(int id) const {
  if (accounts_.find(id) == accounts_.end()) {
    return false;
  }
  return true;
}

bool Bank::is_belong(int id, Client* const& person) {
  return accounts_[id]->is_belong(person);
}

Bank::Bank() : kLimit(0), kPercentage(0) {}

Bank::Bank(std::string name, int limit, int percentage)
    : name_(name), kLimit(limit), kPercentage(percentage) {}

int Bank::open_account(Client*& person, std::string type, int certain_id,
                       bool is_mute) {
  if (accounts_.find(certain_id) != accounts_.end()) {
    return certain_id;
  }
  Account* account;
  if (type == "Debet") {
    account = new DebetAccount(person);
  } else if (type == "Deposit") {
    int period;
    if (!is_mute) {
      std::cout << person->get_name() << ", ";
      std::cout << "specify the deposit period in days";
      std::cin >> period;
    }
    account = new DepositAccount(person, period);
  } else if (type == "Credit") {
    account = new CreditAccount(person, kLimit, kPercentage);
  } else {
    std::cerr << "Incorrect account type\n";
    return -1;
  }
  if (certain_id == -1) {
    accounts_[last_id_] = account;
    if (!is_mute) {
      std::cout << person->get_name() << ", ";
      std::cout << "your account id is " << last_id_ << "\n";
    }
    last_id_++;
    return last_id_ - 1;
  }
  accounts_[certain_id] = account;
  if (!is_mute) {
    std::cout << person->get_name() << ", ";
    std::cout << "your account id is " << certain_id << "\n";
  }
  last_id_ = certain_id + 1;
  return certain_id;
}

void Bank::close_account(int id) {
  if (exist(id)) {
    delete accounts_[id];
    accounts_.erase(accounts_.find(id));
  }
}

void Bank::clear_information() {
  std::vector<int> ids;
  for (auto& pair : accounts_) {
    int id = pair.first;
    ids.push_back(id);
  }
  for (int id : ids) {
    close_account(id);
  }
}

int Bank::balance(int id) { return accounts_[id]->balance(); }

int Bank::withdraw(int id, int sum) { return accounts_[id]->withdraw(sum); }

void Bank::refill(int id, int sum) {
  if (exist(id)) {
    accounts_[id]->refill(sum);
  }
}

void Bank::transaction(int id_sender, int id_receiver, int sum) {
  if (exist(id_sender) && exist(id_receiver)) {
    if (accounts_[id_sender]->withdraw(sum) != 0) {
      accounts_[id_receiver]->refill(sum);
    }
  }
}

void Bank::cancel_last_operation(int id) {
  accounts_[id]->cancel_last_operation();
}

void Bank::set_period(int id, int period) {
  if (accounts_[id]->type() == "Deposit") {
    static_cast<DepositAccount*>(accounts_[id])->set_period(period);
  }
}

int Bank::get_period(int id) {
  return static_cast<DepositAccount*>(accounts_[id])->get_period();
}

std::string Bank::get_type(int id) { return accounts_[id]->type(); }

Bank::~Bank() {
  for (auto pair : accounts_) {
    auto* account = pair.second;
    delete account;
  }
}
