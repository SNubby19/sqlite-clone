#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#ifndef STORAGE_H
#define STORAGE_H
#define sout std::cout

const std::size_t ID_SIZE = sizeof(int);
const std::size_t USERNAME_SIZE = 32;
const std::size_t EMAIL_SIZE = 255;
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
	std::fstream file;
	std::size_t file_length;
	const char *filename;
	char *pages[TABLE_MAX_PAGES];
	void checkFileFail(const std::string &function_of_failure);

public:
	Pager(const char *filename)
	{
		file.open(filename, std::ios::in | std::ios::out | std::ios::ate);

		if (!file.is_open())
		{
			sout << "File failed to open " << filename << "\n";
			file.close();
			file.open(filename, std::ios::in | std::ios::out | std::ios::trunc);	
		}

		if (!file.is_open()) {exit(EXIT_FAILURE);}


		std::streampos file_length = file.tellg();
		std::cout << file_length << std::endl;

		checkFileFail("Pager constructor from command at line 55");

		this->file_length = file_length;

		for (std::size_t i = 0; i < TABLE_MAX_PAGES; i++)
		{
			pages[i] = nullptr;
		}
	}
	std::size_t getFileLength();
	std::fstream *getFile();
	char *getPage(std::size_t page_num);
	char **getPages();
	void flush(std::size_t page_num, std::size_t size);
};

class Row
{
private:
	std::size_t id;
	char username[USERNAME_SIZE + 1];
	char email[EMAIL_SIZE + 1];

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
	std::size_t *getIdPointer();
	char *getUsernamePointer();
	char *getEmailPointer();
};

class Table
{
private:
	std::size_t num_rows;
	char *getRowAddress(std::size_t row_num);
	Pager *pager;

public:
	Table(char *filename)
	{
		Pager *pager = new Pager(filename);
		this->pager = pager;
		num_rows = this->pager->getFileLength() / ROW_SIZE;
		this->num_rows = num_rows;
	}
	~Table()
	{
		std::size_t num_full_pages = num_rows / ROWS_PER_PAGE;

		sout << num_full_pages << "\n";
		for (std::size_t i = 0; i < num_full_pages; i++)
		{
			if (pager->getPages()[i] == nullptr)
			{
				continue;
			}
			sout << "flushing at 128 in destructor\n";
			pager->flush(i, PAGE_SIZE);
			delete pager->getPages()[i];
			pager->getPages()[i] = nullptr;

			sout << pager->getFile()->is_open() << "\n";
		}
		std::size_t page_num = num_full_pages;

		std::size_t num_additional_rows = num_rows % ROWS_PER_PAGE;

		if (num_additional_rows > 0)
		{
			if (pager->getPages()[page_num] != nullptr)
			{
				sout << "flushing at 143 in destructor\n";
				pager->flush(page_num, num_additional_rows * ROW_SIZE);
				delete pager->getPages()[page_num];
				pager->getPages()[page_num] = nullptr;
			}
		}

		if (pager->getFile()->is_open())
		{
			pager->getFile()->close();

			if (pager->getFile()->fail())
			{
				printf("Error closing db file.\n");
				exit(EXIT_FAILURE);
			}
		}

		for (std::size_t i = 0; i < TABLE_MAX_PAGES; i++)
		{
			char *page = pager->getPages()[i];

			if (page)
			{
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
	Pager *getPager();
};

#endif
