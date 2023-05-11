#include <exception>
#include <vector>
#include <string>

using namespace std;

class LeaveShellException : public exception {
	public:
		const char* what() const throw() {
			return "Leaving Shell";
		}
};

class CommandNotRecognizedException : public exception {
	string message; 
	public:
	CommandNotRecognizedException(string cmd){
		message = "Unrecognized Commad: '" + cmd + "'";	
	}
	virtual const char* what () const throw () {
		return message.c_str();
	}
};

class SyntaxErrorException : public exception{
	string message;	
	public:
	SyntaxErrorException(const vector<string>& command) {
		message = "Incorrect syntax with command: " + command[0];	
	}
	virtual const char* what () const throw () {
		return message.c_str();
	}

};
