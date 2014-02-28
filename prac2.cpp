#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "file_parse_exception.h"

using namespace std;

string get_machine_code(string);
int get_instruction_size(string);
bool opcode_exists(string);

std::map<std::string, pair<std::string, std::string> > opcode_map;
std::map<std::string, pair<std::string, std::string> >::iterator m_iter;

int main() {	
	//std::map<std::string, pair<std::string, std::string> > opcode_map;
	//std::map<std::string, pair<std::string, std::string> >::iterator m_iter;
	
	std::string codes[] = { "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", 
				"COMPR","DIV", "DIVF", "DIVR", "FIX", "FLOAT", "HIO", 
				"J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", 
				"LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", 
				"MUL", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", 
				"SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", 
				"STF", "STI", "STL", "STS", "STSW", "STT", "STX", 
				"SUB", "SUBF", "SUBR", "SVC", "TD", "TIO", "TIX", 
				"TIXR", "WD" };
	
	std::string hex_codes[] = { "18", "58", "90", "40", "B4", "28", 
				"88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C", 
				"30", "34", "38", "48", "00", "68", "50", "70", "08", 
				"6C", "74", "04", "D0", "20", "60", "98", "C8", "44", 
				"D8", "AC", "4C", "A4", "A8", "F0", "EC", "0C", "78", 
				"54", "80", "D4", "14", "7C", "E8", "84", "10", "1C", 
				"5C", "94", "B0", "E0", "F8", "2C", "B8", "DC" };
	
	std::string format[] = { "3", "3", "2", "3", "2", "3", "3", "2", "3", 
				"3", "2", "1", "1", "1", "3", "3", "3", "3", "3", "3", "3", 
				"3", "3", "3", "3", "3", "3", "3", "3", "3", "2", "1", "3", 
				"3", "2", "3", "2", "2", "1", "3", "3", "3", "3", "3", "3", 
				"3", "3", "3", "3", "3", "3", "3", "2", "2", "3", "1", "3", 
				"2", "3" };
				
	//cout << codes[1] << endl;
	
	for (int i = 0; i < 59; i++) {
		opcode_map.insert(std::make_pair(codes[i], std::make_pair(hex_codes[i], format[i])));
	}
	m_iter = opcode_map.begin();
	cout << m_iter->first << endl;

	for(m_iter = opcode_map.begin(); m_iter != opcode_map.end(); m_iter++) {
		cout << "Key: " << m_iter->first << " Value: " << m_iter->second.first << endl;
	}
	
	string mac_code = get_machine_code("TIXR");
	cout << mac_code << endl;
	int format_size = get_instruction_size("TIXR");
	cout << format_size << endl;

	//cout << opcode_map.find(codes[0])->second.first << endl;			
return 0;
}

string get_machine_code(string s) {
	if(opcode_exists(s)) {
		string hex = opcode_map.find(s)->second.first;
		return hex;
	}
}

int get_instruction_size(string s) {
	if(opcode_exists(s)) {
		string size = opcode_map.find(s)->second.second;
		int value = atoi(size.c_str());
		return value;
	}
}

bool opcode_exists(string s) {
	if(opcode_map.find(s) == opcode_map.end())
		return false;
	return true;
}


