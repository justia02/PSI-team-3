#include <iostream>
#pragma once
class IllegalStateException :
	public std::exception
{

private:
	const char* errorMessage;
public:
	IllegalStateException(const char *message) : errorMessage(message) {};
	~IllegalStateException() throw() {};
	/// Returns message indicating the error
	const char *what() const throw() { return this->errorMessage; };
};

