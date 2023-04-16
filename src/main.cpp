#include <iostream>

#include "Account.hpp"
#include "Bank.hpp"
#include "Client.hpp"
#include "Interface.hpp"

int main() {
  Interface* interface = new ConsoleInterface();
  interface->main();
}
