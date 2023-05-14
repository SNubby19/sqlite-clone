#include <iostream>
#include <cstdio>
#include <string.h>
#include <string>
#include <vector>
#include "Storage.h"
#include "Exceptions.cpp"

std::fstream *Pager::getFileDescriptor()
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

		if (file_length % PAGE_SIZE)
		{
			num_pages += 1;
		}

		if (page_num <= num_pages)
		{
			file.seekg(page_num * PAGE_SIZE);
			file.read(page, PAGE_SIZE);
			std::streamsize bytes_read = file.gcount();
			if (bytes_read == -1)
			{
				printf("Error reading file: %f\n", errno);
				exit(EXIT_FAILURE);
			}
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
	
	file.write(pages[page_num], size);

	if (file.fail())
	{
		printf("Error writing: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	

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

	printf("(%ld, %s, %s)\n", row->getId(), row->getUsername().c_str(), row->getEmail().c_str());
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
