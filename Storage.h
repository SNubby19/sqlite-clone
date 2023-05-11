#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#ifndef STORAGE_H
#define STORAGE_H

const std::size_t ID_SIZE = sizeof(int);
const std::size_t USERNAME_SIZE = 33;
const std::size_t EMAIL_SIZE = 256;
const std::size_t ID_OFFSET = 0;
const std::size_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const std::size_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const std::size_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const std::size_t PAGE_SIZE = 4096;
#define TABLE_MAX_PAGES 100
const std::size_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const std::size_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Pager {
	private:
		int file_descriptor;
		std::size_t file_length;
		void* pages[TABLE_MAX_PAGES];
	public:
		Pager(const char* filename)	
}


class Row{
	private:
		std::size_t id;
		char username[USERNAME_SIZE]; 
		char email[EMAIL_SIZE];
	public:
		Row(std::vector<std::string> tokens) {
			id = std::stoi(tokens[1]); 
			std::strcpy(username, tokens[2].c_str());
			std::strcpy(email, tokens[3].c_str());
		}
		Row(std::size_t id, std::string username, std::string email) {
			this->id = id;
			std::strcpy(this->username, username.c_str());
			std::strcpy(this->email, email.c_str());
		}
		std::size_t getId();
		std::string getUsername();
		std::string getEmail();
		void* getIdPointer();
		void* getUsernamePointer();
		void* getEmailPointer();

};

class Table{
	private:
		std::size_t num_rows;
		void* pages[TABLE_MAX_PAGES];
		void* getNewRowAddress();
	public:
		Table();	
		void addRow(Row* newRow);
		std::size_t getNumRows();
		void printRows();
		void* getNewRowAddress(std::size_t row_num);
		void readRow(std::size_t row_num);
};

#endif
