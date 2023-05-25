#include <exception>
#include <vector>
#include <string>
#include "Storage.h"

using namespace std;

class LeaveShellException : public exception
{
public:
	const char *what() const throw()
	{
		return "Leaving Shell";
	}
};

class CommandNotRecognizedException : public exception
{
	string message;

public:
	CommandNotRecognizedException(string cmd)
	{
		message = "Unrecognized Command: '" + cmd + "'";
	}
	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};

class SyntaxErrorException : public exception
{
	string message;

public:
	SyntaxErrorException(const vector<string> &command)
	{
		message = "Incorrect syntax with command: " + command[0];
	}
	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};

class PageOutOfBoundsError : public exception
{
	string message;

public:
	PageOutOfBoundsError(std::size_t &page_num)
	{
		message = "Tried to fetch page number out of bounds. " + std::to_string(page_num) + " > " + "%d\n";
	}

	virtual const char *what() const throw()
	{
		return message.c_str();
	}
};
