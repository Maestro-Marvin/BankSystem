#include <iostream>

#include "Account.hpp"
#include "Bank.hpp"
#include "Client.hpp"
#include "Interface.hpp"

int main() {
  ConsoleInterface interface = ConsoleInterface();
  interface.main();
}
