#include <iostream>
#include <string>
#include "Exceptions.cpp"
#include "Statement.h"
#include "Storage.h"

#define sout std::cout

void print_prompt();
void do_meta_command(std::string command, Table* table);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Must supply a database filename.\n");
		exit(EXIT_FAILURE);
	}
	
	char* filename = argv[1];

	std::string command;
	Table* newTable = new Table(filename);

	while(true) {
		print_prompt();

		std::getline(std::cin, command);

		if (command[0] == '.')
		{
			try
			{
				do_meta_command(command, newTable);
				continue;
			}
			catch (LeaveShellException &lse)
			{
				std::cout << lse.what() << std::endl;
				break;
			}
			catch (CommandNotRecognizedException &cnr)
			{
				std::cerr << cnr.what() << std::endl;
				continue;
			}
		}
		else if (command.length() == 0)
		{
			continue;
		}
		else
		{
			Statement *statement = new Statement(command);

			statement->parseStatement();

			try
			{

				statement->executeStatement(newTable);
			}
			catch (CommandNotRecognizedException &cnr)
			{
				std::cerr << cnr.what() << std::endl;
			}
			delete statement;
			statement = nullptr;
		}
	}

	return 0;
}


void print_prompt() {
	std::cout << "db > ";
}

void do_meta_command(std::string command, Table* table) {
	if (command == ".exit") {
		delete table;
		table = nullptr;
		throw LeaveShellException();
	} else {
		throw CommandNotRecognizedException(command);
	}
}


