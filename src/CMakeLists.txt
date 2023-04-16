cmake_minimum_required(VERSION 3.10)

project(BankSystem)
set(SOURCE ${PROJECT_SOURCE_DIR}/Account/Account.cpp
		   ${PROJECT_SOURCE_DIR}/Bank/Bank.cpp
		   ${PROJECT_SOURCE_DIR}/Client/Client.cpp
		   ${PROJECT_SOURCE_DIR}/Interface/Interface.cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(BankSystem ${SOURCE} main.cpp)

target_link_directories(BankSystem PUBLIC
		       "${PROJECT_SOURCE_DIR}/Account"
		       "${PROJECT_SOURCE_DIR}/Bank"
		       "${PROJECT_SOURCE_DIR}/Client"
			   "${PROJECT_SOURCE_DIR}/Interface"
                       )

					   
target_include_directories(BankSystem PUBLIC Account)
target_include_directories(BankSystem PUBLIC Bank)
target_include_directories(BankSystem PUBLIC Client)
target_include_directories(BankSystem PUBLIC Interface)