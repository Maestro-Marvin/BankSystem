#include "Interface.hpp"

void ConsoleInterface::print_message(std::string message) {
  std::cout << message << "\n";
}

void ConsoleInterface::main() {
  bool session = true;
  while (session) {
    Client* client;
    std::vector<User> users;
    user_register(client, users);
    timer_.start_timer();
    bool run = true;
    while (run) {
      print_message("");
      print_message("Choose an option");
      print_message(
          "1.Create bank account\n2.Show "
          "balance\n3.Withdraw\n4.Refill\n5.Transfer money\n"
          "6.Cancel last operation\n7.Set adress\n8.Set "
          "passport\n9.List of accounts\n10.Logout\n"
          "11.Exit");
      int option;
      std::cin >> option;
      print_message("");
      switch (option) {
        case 1:
          create_account(client);
          break;
        case 2:
          balance(client);
          break;
        case 3:
          withdraw(client);
          break;
        case 4:
          refill(client);
          break;
        case 5:
          transaction(client);
          break;
        case 6:
          cancel_last_operation(client);
          break;
        case 7:
          set_adress(client);
          break;
        case 8:
          set_passport(client);
          break;
        case 9:
          list_of_accounts(client);
          break;
        case 10:
          data_storage_.save_information(banks_, users, timer_);
          clear_information();
          run = false;
          break;
        case 11:
          data_storage_.save_information(banks_, users, timer_);
          run = false;
          session = false;
          break;
        default:
          print_message("Incorrect option, try again");
          break;
      }
    }
  }
}