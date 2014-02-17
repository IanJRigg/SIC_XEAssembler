/*  file_parser.cc
    CS530, Spring 2014
*/

#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "file_parser.h"
//#include "file_parse_exception.cc"

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
	in_file_name = file_name;
}

//destructor
file_parser::~file_parser() {

}

void file_parser::read_file() {
//parsing code and storage into vector<struct>

}

string file_parser::get_token(unsigned int line, unsigned int col) {
//gets the token located at
}

void file_parser::print_file() {
//prints the input file in propper format
}

int file_parser::size() {
//returns the number of lines in the file
	return file_line.size();
}


