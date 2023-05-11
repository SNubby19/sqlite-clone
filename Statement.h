#include <string>
#include <vector>
#include "Storage.h"

class Statement {
	private:
		std::string statementString;
		std::vector<std::string> tokens;
	public:
		Statement(std::string newStatement);
		void setStatement(std::string newStatement);
		std::string getStatement();
		void parseStatement();
		void executeStatement(Table* table);	
		void executeInsert(Table* table);
		void executeSelect(Table* table);
};
