/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Exception class for symtab
    CS530 Spring 2014
    Team Florida
    created originally by Alan Riggins
*/    

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H
#include <string> 

using namespace std;

class symtab_exception {

public:
    symtab_exception(string s) {
        message = s;        
        }
        
    symtab_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};    

#endif