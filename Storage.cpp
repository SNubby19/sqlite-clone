#include <iostream>
#include <cstdio>
#include <string.h>
#include <string>
#include <vector>
#include "Storage.h"
#include "Exceptions.cpp"

#define sout std::cout

std::fstream *Pager::getFile()
{
	return &(file);
}

std::size_t Pager::getFileLength()
{
	return file_length;
}

char **Pager::getPages()
{
	return pages;
}

void Pager::checkFileFail(const std::string &function_of_failure)
{
	if (file.fail())
	{
		sout << "file failed at " << function_of_failure << "\n";
	}
}

char *Pager::getPage(std::size_t page_num)
{
	if (page_num > TABLE_MAX_PAGES)
	{
		throw PageOutOfBoundsError(page_num);
	}

	if (pages[page_num] == nullptr)
	{
		char *page = new char[PAGE_SIZE];
		std::size_t num_pages = file_length / PAGE_SIZE;

		// if (file_length % PAGE_SIZE != 0)
		// {
		// 	num_pages++;
		// }

	

		if (page_num <= num_pages && file_length != 0)
		{
			file.seekg(page_num * PAGE_SIZE, std::ios::beg);

			sout << num_pages << "\n";

			sout << PAGE_SIZE << "\n";

			sout << file_length << "\n";

			if (file_length >= PAGE_SIZE)
			{
				file.read(page, PAGE_SIZE);
			} else {
				file.read(page, file_length);
			}


			if (file.fail()) {
				sout << "I might give up at this point \n";
			}

		// if (file.fail())
			// {
				// if (file.eof())
				// {
				// 	sout << "Reached end of file.\n";

				// 	file.close();

				// 	if (file.is_open()) {
				// 		sout << "file is still open ??? \n";
				// 	}

				// 	if (file.fail()) {
				// 		sout << "Closing failed ??\n";
				// 	}	
				
				// 	file.open(filename, std::ios::in | std::ios::out);		

				// 	if (file.fail()) {
				// 		sout << "File at end of line after reopening??\n";
				// 	}	
				// }
				// else
				// {
				// 	sout << "can't identify error, here is the errno: " << errno << "\n";
				// }
		// 	}
		}

		pages[page_num] = page;
	}
	return pages[page_num];
}

void Pager::flush(std::size_t page_num, std::size_t size)
{
	if (pages[page_num] == nullptr)
	{
		printf("Tried to flush null page\n");
		exit(EXIT_FAILURE);
	}

	file.seekp(page_num * PAGE_SIZE, std::ios::beg);

	if (file.fail())
	{
		checkFileFail("flush ln89");
		printf("Failed to seek write position in the file %d\n", errno);
		exit(EXIT_FAILURE);
	}

	file.write(pages[page_num], size);
	checkFileFail("flush ln97");

	if (file.fail())
	{
		std::cerr << "Error: Write operation failed.\n";
	}

	sout << (bool)file.fail() << "\n";
}

std::size_t Row::getId()
{
	return id;
}

std::string Row::getUsername()
{
	return username;
}

std::string Row::getEmail()
{
	return email;
}

std::size_t *Row::getIdPointer()
{
	return &id;
}

char *Row::getUsernamePointer()
{
	return username;
}

char *Row::getEmailPointer()
{
	return email;
}

Pager *Table::getPager()
{
	return pager;
}

char *Table::getRowAddress(std::size_t row_num)
{
	std::size_t page_num = row_num / ROWS_PER_PAGE;
	char *page = pager->getPage(page_num);

	std::size_t row_offset = row_num % ROWS_PER_PAGE;
	std::size_t byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}

void Table::addRow(Row *newRow)
{
	if (num_rows >= TABLE_MAX_ROWS)
	{
		std::cout << "Table is full, cannot execute insert." << std::endl;
		return;
	}

	char *newRowAddress = getRowAddress(num_rows);

	memcpy(newRowAddress + ID_OFFSET, newRow->getIdPointer(), ID_SIZE);
	memcpy(newRowAddress + USERNAME_OFFSET, newRow->getUsernamePointer(), USERNAME_SIZE);
	memcpy(newRowAddress + EMAIL_OFFSET, newRow->getEmailPointer(), EMAIL_SIZE);

	num_rows++;

	return;
}

void Table::readRow(std::size_t row_num)
{
	char *row_address = getRowAddress(row_num);

	Row *row = new Row(0, "", "");

	memcpy(row->getIdPointer(), row_address + ID_OFFSET, ID_SIZE);
	memcpy(row->getUsernamePointer(), row_address + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(row->getEmailPointer(), row_address + EMAIL_OFFSET, EMAIL_SIZE);

	printf("(%lld, %s, %s)\n", row->getId(), row->getUsername().c_str(), row->getEmail().c_str());

	delete row;
	row = nullptr;
}

void Table::printRows()
{
	for (std::size_t i = 0; i < num_rows; i++)
	{
		readRow(i);
	}
}

std::size_t Table::getNumRows()
{
	return num_rows;
}
