#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include "Statement.h"
#include "Exceptions.cpp"
#include "Storage.h"

Statement::Statement(std::string newStatement) : statementString(newStatement) {}

void Statement::setStatement(std::string newStatement) {
	statementString = newStatement;
}

std::string Statement::getStatement() {
	return statementString;
}

void Statement::parseStatement() {
	std::istringstream iss(statementString);
	std::string word;
	while (getline(iss, word, ' ')) {
		tokens.push_back(word);
	}
	return;
}

void Statement::executeStatement(Table* table) {
	if (tokens[0] == "insert") {
		try {
			executeInsert(table);
		} catch (SyntaxErrorException &see) {
			std::cerr << see.what() << std::endl;
		} catch (std::invalid_argument& e) {
			std::cout << "Invalid argument " << tokens[1] << std::endl;
		}	

	} else if (tokens[0] == "select") {
		executeSelect(table);
	} else {
		throw CommandNotRecognizedException(statementString);	
	}
}

void Statement::executeSelect(Table* table) {
	table->printRows();
	std::cout << "Executed." << std::endl;

	return;
}

void Statement::executeInsert(Table* table) {
	if (tokens.size() < 4) {
		throw SyntaxErrorException(tokens);
	}

	Row* newRow = new Row(tokens);

	table->addRow(newRow);
	std::cout << "Executed." << std::endl;

	return;
}
