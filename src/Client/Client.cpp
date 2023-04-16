#include "Client.hpp"

#include <iostream>
#include <string>

Client::Client(const std::string& first_name, const std::string& second_name,
               const std::string& adress, const std::string& passport)
    : firstName_(first_name),
      secondName_(second_name),
      adress_(adress),
      passport_(passport) {}

std::string Client::GetName() const { return firstName_ + " " + secondName_; }

void Client::SetAdress(const std::string& adress) { adress_ = adress; }

void Client::SetPassport(const std::string& passport) { passport_ = passport; }

bool Client::IsReliable() const {
  if (adress_ == "-" && passport_ == "-") {
    return false;
  }
  return true;
}

Client* BuildClient() {
  std::string firstName;
  std::cout << "Your firts name :\n";
  std::cin >> firstName;
  std::string secondName;
  std::cout << "Your second name :\n";
  std::cin >> secondName;
  std::string adress;
  std::cout << "Your adress :\n";
  std::cin >> adress;
  std::string passport;
  std::cout << "Your passport :\n";
  std::cin >> passport;
  return new Client(firstName, secondName, adress, passport);
}
