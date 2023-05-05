#include "Client.hpp"

Client::Client(const std::string& first_name, const std::string& second_name, const std::string& phone,
               const std::string& adress, const std::string& passport)
    : firstName_(first_name),
      secondName_(second_name),
      phone_(phone),
      adress_(adress),
      passport_(passport) {}

Client* Client::build_client() {
  std::string first_name;
  std::cout << "Your firts name :\n";
  std::cin >> first_name;
  std::string second_name;
  std::cout << "Your second name :\n";
  std::cin >> second_name;
  std::string phone;
  std::cout << "Your phone :\n";
  std::cin >> phone;
  std::string adress;
  std::cout << "Your adress :\n";
  std::cin >> adress;
  std::string passport;
  std::cout << "Your passport :\n";
  std::cin >> passport;
  return new Client(first_name, second_name, phone, adress, passport);
}

std::vector<std::string> Client::get_info() {
    std::vector<std::string> ans;
    ans.push_back(firstName_);
    ans.push_back(secondName_);
    ans.push_back(phone_);
    ans.push_back(adress_);
    ans.push_back(passport_);
    return ans;
  }

std::string Client::get_name() const { return firstName_ + " " + secondName_; }

std::string Client::get_phone() const { return phone_; }

void Client::set_phone(const std::string& phone) { phone_ = phone; }

void Client::set_adress(const std::string& adress) { adress_ = adress; }

void Client::set_passport(const std::string& passport) { passport_ = passport; }

bool Client::is_reliable() const {
  if (phone_ == "-" && adress_ == "-" && passport_ == "-") {
    return false;
  }
  return true;
}
