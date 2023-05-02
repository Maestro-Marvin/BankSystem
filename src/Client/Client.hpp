#pragma once
#include <iostream>
#include <string>
#include <vector>

class Client {
  std::string firstName_;
  std::string secondName_;
  std::string phone_;
  std::string adress_;
  std::string passport_;

 public:
  Client() {}

  Client(const std::string& first_name, const std::string& second_name, const std::string& phone,
         const std::string& adress, const std::string& passport);

  static Client* build_client();

  std::vector<std::string> get_info();

  std::string get_name() const;

  std::string get_phone() const;

  void set_phone(const std::string& phone);

  void set_adress(const std::string& adress);

  void set_passport(const std::string& passport);

  bool is_reliable() const;

  ~Client() {}
};
