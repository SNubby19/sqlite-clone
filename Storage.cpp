#include <iostream>
#include <cstdio>
#include <string.h>
#include <string>
#include <vector>
#include "Storage.h"


std::size_t Row::getId() {
	return id;
}

std::string Row::getUsername() {
	return username;
}

std::string Row::getEmail() {
	return email;
}

void* Row::getIdPointer() {
	return &id;
}

void* Row::getUsernamePointer() {
	return &username;
}

void* Row::getEmailPointer() {
	return &email;
}


Table::Table () {
	num_rows = 0;

	for (std::size_t i = 0; i < TABLE_MAX_PAGES; i++) {
		pages[i] = nullptr;
	}
}

void* Table::getNewRowAddress(std::size_t row_num) {
	std::size_t page_num = row_num/ ROWS_PER_PAGE;
	void* page = pages[page_num];

	if (!page){
		pages[page_num] = static_cast<void*>(new char[PAGE_SIZE]);	
		page = pages[page_num];
	}

	std::size_t row_offset = row_num % ROWS_PER_PAGE;
	std::size_t byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}

void Table::addRow(Row* newRow) {
	if(num_rows >= TABLE_MAX_ROWS) {
		std::cout << "Table is full, cannot execute insert." << std::endl;
		return;
	}

	void* newRowAddress = getNewRowAddress(num_rows);

	memcpy(newRowAddress + ID_OFFSET, newRow->getIdPointer(), ID_SIZE);	
	memcpy(newRowAddress + USERNAME_OFFSET, newRow->getUsernamePointer(), USERNAME_SIZE);	
	memcpy(newRowAddress + EMAIL_OFFSET, newRow->getEmailPointer(), EMAIL_SIZE);	

	num_rows++;

	return;
}

void Table::readRow(std::size_t row_num) {
	void* row_address = getNewRowAddress(row_num);

	Row* row = new Row(0, "", "");
	memcpy(row->getIdPointer(), row_address + ID_OFFSET, ID_SIZE);	
	memcpy(row->getUsernamePointer(), row_address + USERNAME_OFFSET, USERNAME_SIZE);	
	memcpy(row->getEmailPointer(), row_address + EMAIL_OFFSET, EMAIL_SIZE);	

	printf("(%ld, %s, %s)\n", row->getId(), row->getUsername().c_str(), row->getEmail().c_str());	

}

void Table::printRows() {
	for (std::size_t i = 0; i < num_rows; i++) {
		readRow(i);
	}	
}

std::size_t Table::getNumRows() {
	return num_rows;
}

