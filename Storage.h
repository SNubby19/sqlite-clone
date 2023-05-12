#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

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

class Pager
{
private:
	std::fstream file_descriptor;
	std::size_t file_length;
	void *pages[TABLE_MAX_PAGES];

public:
	Pager(const char *filename)
	{
		this->file_descriptor.open(filename, std::ios::in | std::ios::out | std::ios::trunc);

		if (!file_descriptor.is_open())
		{
			std::cerr << "File did not open\n";
			exit(1);
		}

		file_descriptor.seekg(0, file_descriptor.end);
		off_t file_length = file_descriptor.tellg();

		this->file_length = file_length;

		for (std::size_t i = 0; i < TABLE_MAX_PAGES; i++)
		{
			pages[i] = nullptr;
		}
	}
	std::size_t getFileLength();
	std::fstream getFileDescriptor();
	void *getPage(std::size_t page_num);
	void **getPages();
	void flush(std::size_t page_num, std::size_t size);
};

class Row
{
private:
	std::size_t id;
	char username[USERNAME_SIZE];
	char email[EMAIL_SIZE];

public:
	Row(std::vector<std::string> tokens)
	{
		id = std::stoi(tokens[1]);
		std::strcpy(username, tokens[2].c_str());
		std::strcpy(email, tokens[3].c_str());
	}
	Row(std::size_t id, std::string username, std::string email)
	{
		this->id = id;
		std::strcpy(this->username, username.c_str());
		std::strcpy(this->email, email.c_str());
	}
	std::size_t getId();
	std::string getUsername();
	std::string getEmail();
	void *getIdPointer();
	void *getUsernamePointer();
	void *getEmailPointer();
};

class Table
{
private:
	std::size_t num_rows;
	void *getNewRowAddress(std::size_t row_num);
	Pager *pager;

public:
	Table(char *filename)
	{
		Pager *pager = new Pager(filename);
		this->pager = pager;
		num_rows = pager->getFileLength() / ROW_SIZE;
		this->num_rows = num_rows;
	}
	~Table() {
		std::size_t num_full_pages = num_rows/ROWS_PER_PAGE;

		for (std::size_t i = 0; i < num_full_pages; i++) {
			if(pager->getPages()[i] == nullptr) {
				continue;
			}
			pager->flush(i, PAGE_SIZE);
			delete pager->getPages()[i];
			pager->getPages()[i] = nullptr;
		}

		std::size_t num_additional_rows = num_rows % ROWS_PER_PAGE;

		if (num_additional_rows > 0) {
			std::size_t page_num = num_full_pages;
			if (pager->getPages()[page_num] != nullptr) {
				pager->flush(page_num, num_additional_rows * ROW_SIZE);
				delete pager->getPages()[page_num];
				pager->getPages()[page_num] = nullptr;
			}
		}

		pager->getFileDescriptor().close();

		if(!pager->getFileDescriptor()) {
			printf("Error closing db file.\n");
			exit(EXIT_FAILURE);
		}

		for(std::size_t i = 0; i < TABLE_MAX_PAGES; i++) {
			void* page = pager->getPages()[i];

			if (page) {
				delete page;
				pager->getPages()[i] = nullptr;
			}
		}
		delete pager;
		pager = nullptr;


	};
	void addRow(Row *newRow);
	std::size_t getNumRows();
	void printRows();
	void readRow(std::size_t row_num);
};

#endif
