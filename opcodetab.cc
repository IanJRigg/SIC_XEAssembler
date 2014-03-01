/*
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Prog2: opcodetab.cc
    CS530, Spring 2014
    Team Florida
*/

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

//constructor creates a new dictionary structure and loads all of the opcodes for 
// the SIC/XE architecture into the table. 
opcodetab::opcodetab() {
    //Create list of opcodes
    std::string codes[] = { "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", 
			"COMPR","DIV", "DIVF", "DIVR", "FIX", "FLOAT", "HIO", 
			"J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", 
			"LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", 
			"MUL", "MULF", "MULR", "NORM", "OR", "RD", "RMO", "RSUB", 
			"SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", 
			"STF", "STI", "STL", "STS", "STSW", "STT", "STX", 
			"SUB", "SUBF", "SUBR", "SVC", "TD", "TIO", "TIX", 
			"TIXR", "WD" };
    //Create list of hex values	
    std::string hex_codes[] = { "18", "58", "90", "40", "B4", "28", 
			"88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C", 
			"30", "34", "38", "48", "00", "68", "50", "70", "08", 
			"6C", "74", "04", "D0", "20", "60", "98", "C8", "44", 
			"D8", "AC", "4C", "A4", "A8", "F0", "EC", "0C", "78", 
			"54", "80", "D4", "14", "7C", "E8", "84", "10", "1C", 
			"5C", "94", "B0", "E0", "F8", "2C", "B8", "DC" };
    //Create list of format sizes	
    std::string format[] = { "3", "3", "2", "3", "2", "3", "3", "2", "3", 
			"3", "2", "1", "1", "1", "3", "3", "3", "3", "3", "3", "3", 
			"3", "3", "3", "3", "3", "3", "3", "3", "3", "2", "1", "3", 
			"3", "2", "3", "2", "2", "1", "3", "3", "3", "3", "3", "3", 
			"3", "3", "3", "3", "3", "3", "3", "2", "2", "3", "1", "3", 
			"2", "3" };
    //Create list of invalid format 4 types                    
    std::string invalid_format_four[] = {"ADDR","CLEAR","COMPR","DIVR","FIX","FLOAT",
                        "HIO","MULR","NORM","RMO","SHIFTL","SHIFTR","SIO","SUBR",
                        "SVC","TIO","TIXR"};
     //Insert invalid format four items into map                   
    for(int i = 0; i < INVALID_FORMAT_FOUR_NUM; i++) {
	opcode_format_map.insert(std::make_pair(invalid_format_four[i],"test"));
    }
    //Insert opcodes into map		
    for(int i = 0; i < NUMBER_OF_OPCODES; i++) {
	opcode_map.insert(std::make_pair(codes[i], std::make_pair(hex_codes[i], format[i])));
    }	
}

string opcodetab::get_machine_code(string s) {
    string tmp = s;
    stringstream ss;
    int format =0;//Tracks if opcode is format 4
    //Change incomming string to all uppercase
    std::transform(tmp.begin(), tmp.end(),tmp.begin(), ::toupper);
    
    //Check if opcode is attempting format 4 and if so increment counter
    if(s[0]=='+'){
        tmp = tmp.substr(1,tmp.size());
        format++;
    }
    //Check if formatting has been adjusted, if so verify code is valid for format 4
    if(format==1 && invalid_format(tmp)){
        ss<<"Opcode "<<s<<": not valid for format 4"<<endl;
        throw opcode_error_exception(ss.str());    
    }
    //Verify opcode exists in map
    if(opcode_exists(tmp)) {

	string hex = opcode_map.find(tmp)->second.first;
	return hex;
    }
    else
	throw opcode_error_exception("Opcode entered does not exist");
}

int opcodetab::get_instruction_size(string s) {
    stringstream ss;
    string tmp = s;
    int format = 0;
    
    //Transform to uppercase        
    std::transform(tmp.begin(), tmp.end(),tmp.begin(), ::toupper);
    //Check if format 4
    if(s[0]=='+'){
        tmp = tmp.substr(1,tmp.size());
        format++;
    }
    //Check if valid format 4
    if(format==1 && invalid_format(tmp)){
        ss<<"Opcode "<<s<<": not valid for format 4"<<endl;
        throw opcode_error_exception(ss.str());
    }
    //Check if the opcode exists in the map
    if(opcode_exists(tmp)) {
	string size = opcode_map.find(tmp)->second.second;
	int value = atoi(size.c_str());
        value=value+format;
	return value;
    }
    else{
	ss<<"Opcode "<<s<<" does not exist"<<endl;
        throw opcode_error_exception(ss.str());    
    }
}

//Method to verify opcode does/does not exist in map
bool opcodetab::opcode_exists(string s) {
    if(opcode_map.find(s) == opcode_map.end())
	return false;
    return true;
}
//Method to verify if opcode is not eligible for format 4
bool opcodetab::invalid_format(string s){
    if(opcode_format_map.find(s) == opcode_format_map.end())
        return false;
    return true;
}


