#include <iostream>
#include <string>
#include "Exceptions.cpp"
#include "Statement.h"
#include "Storage.h"


void print_prompt();
void do_meta_command(std::string command);

int main() {
	std::string command;
	Table* newTable = new Table();


	while(true) {
		print_prompt();

		std::getline(std::cin, command);
		if (command[0] == '.') {
			try {
				do_meta_command(command);
				continue;
			} catch (LeaveShellException& lse) {
				std::cout << lse.what() << std::endl;
				break;
			}
			catch (CommandNotRecognizedException& cnr) {
				std::cerr << cnr.what() << std::endl;	
				continue;
			}	

		} else {
			Statement *statement = new Statement(command);

			statement->parseStatement();
			
			try {

				statement->executeStatement(newTable);
			} catch (CommandNotRecognizedException& cnr) {
				std::cerr << cnr.what() << std::endl;
			}
			delete statement;
			statement = nullptr;
		}
	}	

	delete newTable;
	newTable = nullptr;
	return 0;
}


void print_prompt() {
	std::cout << "db > ";
}

void do_meta_command(std::string command) {
	if (command == ".exit") {
		throw LeaveShellException();
	} else {
		throw CommandNotRecognizedException(command);
	}
}


