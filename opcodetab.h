/*
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Prog2: opcodetab.h
    CS530, Spring 2014
    Team Florida
*/

#ifndef OPCODETAB_H
#define OPCODETAB_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <utility>

#define NUMBER_OF_OPCODES 59
#define INVALID_FORMAT_FOUR_NUM 17

using namespace std;

class opcodetab {
    public:
        // ctor
        // creates a new dictionary structure and loads all of the opcodes for 
        // the SIC/XE architecture into the table.  Use the STL map for this.
        opcodetab(); 
        
        // takes a SIC/XE opcode and returns the machine code 
        // equivalent as a two byte string in hexadecimal.
        // Example:  get_machine_code("ADD") returns the value 18
        // Note that opcodes may be prepended with a '+'.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.
        string get_machine_code(string);  
        
        // takes a SIC/XE opcode and returns the number of bytes 
        // needed to encode the instruction, which is an int in
        // the range 1..4.
        // NOTE: the opcode must be prepended with a '+' for format 4.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.        
        int get_instruction_size(string);

        bool opcode_exists(string);
                        
    private:
        // your variables and private methods go here
	static const string codes[NUMBER_OF_OPCODES];
    static const string hex_codes[NUMBER_OF_OPCODES];
	static const string format[NUMBER_OF_OPCODES];
    static const string invalid_format_four[INVALID_FORMAT_FOUR_NUM];
    
	map<string, pair<string, string> > opcode_map;
	map<string, pair<string, string> >::iterator m_iter;
    map<string,string> opcode_format_map;        
	map<string,string>::iterator format_iter;
	
	
    bool invalid_format(string);
    stringstream ss;
};
#endif
