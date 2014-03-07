/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Prog2: opcode_error_exception.h
    Exception class for opcodetab
    CS530 Spring 2014
    Team Florida
    created originally by Alan Riggins
*/ 

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H
#include <string>

using namespace std;

class opcode_error_exception {

public:
	opcode_error_exception(string s) {
		message = s;
	}
	
	opcode_error_exception() {
		message = "An error has occured";
	}
	
	string getMessage() {
		return message;
	}
	
private:
	string message;
};

#endif
