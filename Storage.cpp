#include <iostream>
#include <cstdio>
#include <string.h>
#include <string>
#include <vector>
#include "Storage.h"
#include "Exceptions.cpp"

void *Pager::getPage(std::size_t page_num)
{
	if (page_num > TABLE_MAX_PAGES)
	{
		throw PageOutOfBoundsError(page_num);
	}

	if (pages[page_num] == nullptr)
	{
		void *page = static_cast<void *>(new char[PAGE_SIZE]);
		std::size_t num_pages = file_length / PAGE_SIZE;

		if (file_length % PAGE_SIZE)
		{
			num_pages += 1;
		}

		if (page_num <= num_pages)
		{
			file_descriptor.seekg(page_num * PAGE_SIZE);
			file_descriptor.read(static_cast<char *>(page), PAGE_SIZE);
			std::streamsize bytes_read = file_descriptor.gcount();
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

void Pager::flush(std::size_t page_num, std::size_t size) {
	if (pages[page_num] == nullptr) {
		printf("Tried to flush null page\n");
		exit(EXIT_FAILURE);
	}

	file_descriptor.seekp(page_num*PAGE_SIZE);

	if (file_descriptor.fail()) {
		printf("Error seeking: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	if(!file_descriptor.write(static_cast<char*>(pages[page_num]), size)) {
		printf("Error writing %d\n", errno);
		exit(EXIT_FAILURE);
	}
}

std::size_t Pager::getFileLength()
{
	return file_length;
}

void **Pager::getPages()
{
	return pages;
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

void *Row::getIdPointer()
{
	return &id;
}

void *Row::getUsernamePointer()
{
	return &username;
}

void *Row::getEmailPointer()
{
	return &email;
}

void *Table::getNewRowAddress(std::size_t row_num)
{
	std::size_t page_num = row_num / ROWS_PER_PAGE;
	void *page = pager->getPage(page_num);

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

	void *newRowAddress = getNewRowAddress(num_rows);

	memcpy(newRowAddress + ID_OFFSET, newRow->getIdPointer(), ID_SIZE);
	memcpy(newRowAddress + USERNAME_OFFSET, newRow->getUsernamePointer(), USERNAME_SIZE);
	memcpy(newRowAddress + EMAIL_OFFSET, newRow->getEmailPointer(), EMAIL_SIZE);

	num_rows++;

	return;
}

void Table::readRow(std::size_t row_num)
{
	void *row_address = getNewRowAddress(row_num);

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
