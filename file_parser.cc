/*  file_parser.cc
    CS530, Spring 2014
*/

#include "file_parser.h"
#include "file_parse_exception.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string> 

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
	in_file_name = file_name;
	set_operand_set(0);
	opcode_set = 0;
	start = 0;
}

//destructor
file_parser::~file_parser() {
}

void file_parser::read_file() {
//parsing code and storage into vector<struct>
	//ifstream infile = new ifstream();
	infile.open(in_file_name.c_str(), ios::in);
	if (!infile) {
		throw file_parse_exception("Error opening source code file.");
	}
	
	while (!infile.eof()) {
        getline(infile, line);
	set_operand_set(0);
        //operand_set=0;
        opcode_set=0;
        start =0;
        contents.push_back(parsed_line());
        for(unsigned int i=0; i< line.size();i++){
            if(line[i] == '.'){
                contents[v_counter].comment = line.substr(i,line.size()-i+1);
                break;
            }
            if((!isspace(line[i])) && (i==0)){
                while(!isspace(line[i])){
                    i++;
                    }
                contents[v_counter].label= line.substr(0,i);
                continue;
            }
            if((!isspace(line[i]))&&(!opcode_set)){
                start = i;
                while(!isspace(line[i])){
                i++;
                }
                contents[v_counter].opcode=line.substr(start,i-start);
                opcode_set =1;
                continue;
            }
            if((!isspace(line[i]))&&(opcode_set)&&(!operand_set)){
                start = i;
                while(!isspace(line[i])){
                i++;
                }
                contents[v_counter].operand=line.substr(start,i-start);
                operand_set =1;
                continue;
            }
            if(isspace(line[i])){
                continue;
            }
            //Need to code this to throw exception
            if((line[i]!='.')&&(!isspace(line[i]))){
                cout<<"Error with "<<line.substr(i,10)<<endl;
                break;
            }


        }
        v_counter++;
    }

    infile.close(); 
}

string file_parser::get_token(unsigned int r, unsigned int c) {
//gets the token located at
	unsigned int column = c;
	unsigned int row = r;
	string token;
	
	switch (column) {
		case 0:
			return contents[row].label;
		case 1:
			return contents[row].opcode;
		case 2:
			return contents[row].operand;
		case 3:
			return contents[row].comment;
		default:
			return "error";
	}
}

void file_parser::print_file() {
//prints the input file in proper format
	for (int i = 0; i < v_counter; i++) {
		cout << contents[i].label << "\t";
		cout << contents[i].opcode << "\t";
		cout << contents[i].operand << "\t";
		cout << contents[i].comment << endl;
	}
}

int file_parser::size() {
//returns the number of lines in the  source code file
	return contents.size();
}

void file_parser::set_operand_set(int a) {
	operand_set = a;
}
