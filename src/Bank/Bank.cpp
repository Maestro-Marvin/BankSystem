#include "Bank.hpp"

#include <iostream>
#include <map>
#include <string>

#include "Account.hpp"

std::vector<std::string> Bank::GetAccounts(Client* client) {
  std::vector<std::string> ans;
  for (auto pair : accounts_) {
    if (pair.second->GetClient() == client) {
      ans.push_back(std::to_string(pair.first));
      ans.push_back(pair.second->Type());
      ans.push_back(std::to_string(Balance(pair.first)));
    }
  }
  return ans;
}

bool Bank::Exist(int id) const {
  if (accounts_.find(id) == accounts_.end()) {
    std::cerr << "There isn't this account\n";
    return false;
  }
  return true;
}

Bank::Bank() : limit_(0), percentage_(0) {}

Bank::Bank(std::string name, int limit, int percentage)
    : name_(name), limit_(limit), percentage_(percentage) {}

int Bank::OpenAccount(Client* person, std::string type, int certain_id,
                      bool is_mute) {
  if (accounts_.find(certain_id) != accounts_.end()) {
    return certain_id;
  }
  Account* account;
  if (type == "Debet") {
    account = new DebetAccount(person);
  } else if (type == "Deposit") {
    int period;
    std::cout << person->GetName() << ", ";
    std::cout << "specify the period ";
    std::cin >> period;
    account = new DepositAccount(person, period);
  } else if (type == "Credit") {
    account = new CreditAccount(person, limit_, percentage_);
  } else {
    std::cerr << "Incorrect account type\n";
    return -1;
  }
  if (certain_id == -1) {
    accounts_[last_id] = account;
    if (!is_mute) {
      std::cout << person->GetName() << ", ";
      std::cout << "your account id is " << last_id << "\n";
    }
    last_id++;
    return last_id - 1;
  } else {
    accounts_[certain_id] = account;
    if (is_mute) {
      std::cout << person->GetName() << ", ";
      std::cout << "your account id is " << certain_id << "\n";
    }
    return certain_id;
  }
}

void Bank::CloseAccount(int id) {
  if (Exist(id)) {
    delete accounts_[id];
    accounts_.erase(accounts_.find(id));
  }
}

int Bank::Balance(int id) {
  if (Exist(id)) {
    return accounts_[id]->Balance();
  }
}

int Bank::Withdraw(int id, int sum) {
  if (Exist(id)) {
    return accounts_[id]->Withdraw(sum);
  }
}

void Bank::Refill(int id, int sum) {
  if (Exist(id)) {
    accounts_[id]->Refill(sum);
  }
}

void Bank::Transaction(int id_sender, int id_receiver, int sum) {
  if (Exist(id_sender) && Exist(id_receiver)) {
    if (accounts_[id_sender]->Withdraw(sum) != 0) {
      accounts_[id_receiver]->Refill(sum);
    }
  }
}

void Bank::CancelLastOperation(int id) {
  if (Exist(id)) {
    accounts_[id]->CancelLastOperation();
  }
}

void Bank::DecreasePeriod(int id) {
  DepositAccount* ptr = dynamic_cast<DepositAccount*>(accounts_[id]);
  if (ptr == nullptr) {
    std::cerr << "It isn't a deposit account";
  } else {
    ptr->DecreasePeriod();
  }
}

Bank::~Bank() {
  for (auto p : accounts_) {
    auto account = p.second;
    delete account;
  }
}
