/*
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    opcodetab.cc
    CS530, Spring 2014
*/

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

//constructor creates a new dictionary structure and loads all of the opcodes for 
// the SIC/XE architecture into the table. 
opcodetab::opcodetab() {
	string codes[59] = { "ADD", "ADDF", "ADDR", "AND", "CLEAR", 
		"COMP", "COMPF", "COMPR","DIV", "DIVF", "DIVR", "FIX", "FLOAT", "HIO", 
		"J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", 
		"LDS", "LDT", "LDX", "LPS", "MULF", "MULR", "NORM", "OR", "RD", "RMO", 
		"RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", "STF",
		"STI", "STL", "STS", "STSW", "STT", "STX", "SUB", "SUBF", "SUBR",
		"SVC", "TD", "TIO", "TIX", "TIXR", "WD" };
	
	string hex_codes[59] = { "18", "58", "90", "40", "B4", "28", 
				"88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C", 
				"30", "34", "38", "48", "00","68", "50", "70", "08", 
				"6C", "74", "04", "D0", "20", "60", "98", "C8", "44", 
				"D8", "AC", "4C", "A4", "A8", "F0", "EC","0C", "78", 
				"54", "80", "D4", "14", "7C", "E8", "84", "10",
				"1C", "5C", "94", "B0", "E0", "F8", "2C", "B8", "DC" };
	
	string format[59] = { "3", "3", "2", "3", "2", "3", "3", "2", "3", 
				"3", "2", "1", "1", "1", "3", "3", "3", "3", "3", "3", "3", 
				"3", "3", "3", "3", "3", "3", "3", "3", "3", "2", "1", "3", 
				"3", "2", "3", "2", "2", "1", "3", "3", "3", "3", "3", "3", 
				"3", "3", "3", "3", "3", "3", "3", "2", "2", "3", "1", "3", 
				"2", "3" };
			
	//need to fix for loop		
	for(int i = 0; i < 59; i++) {
		if(opcode_exists[codes[i] ]) {
			opcode_map[codes[i] ] = 
		}
	}
}

string opcodetab::get_machine_code(string s) {

}

int opcodetab::get_instruction_size(string s) {

}

bool opcodetab::opcode_exists(string s) {
	if(opcode_map.find(s) == opcode_map.end())
		return false;
	return true;
}
