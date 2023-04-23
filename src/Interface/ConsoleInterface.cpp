#include "Interface.hpp"

void ConsoleInterface::print_message(std::string message) {
  std::cout << message << "\n";
}

void ConsoleInterface::main() {
  Client* client;
  std::vector<User> users;
  user_register(client, users);
  bool run = true;
  while (run) {
    print_message("Choose an option");
    print_message(
        "1.Create bank account\n2.Show "
        "balance\n3.Withdraw\n4.Refill\n5.Transfer money\n"
        "6.Cancel last operation\n7.Decrease period\n8.Set adress\n9.Set "
        "passport\n"
        "10.Exit");
    int option;
    std::cin >> option;
    switch (option) {
      case 1:
        create_account(client);
        break;
      case 2:
        balance();
        break;
      case 3:
        withdraw();
        break;
      case 4:
        refill();
        break;
      case 5:
        transaction();
        break;
      case 6:
        cancel_last_operation();
        break;
      case 7:
        decrease_period();
        break;
      case 8:
        set_adress(client);
        break;
      case 9:
        set_passport(client);
        break;
      case 10:
        data_storage_.save_information(banks_, users);
        run = false;
        break;
      default:
        print_message("Incorrect option, try again");
        break;
    }
  }
}