/*  file_parser.cc
    CS530, Spring 2014
*/

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
	in_file_name = file_name;
}

//destructor
//<COMMENT TO BE REMOVED> Removed casting of class methods, unnecessary in .cc since handled by .h
file_parser::~file_parser() {

}

file_parser::read_file() {
//parsing code and storage into vector<struct>

}

file_parser::get_token(unsigned int line, unsigned int col) {
//gets the token located at
}

file_parser::print_file() {
//prints the input file in proper format
}

file_parser::size() {
//returns the number of lines in the file
	return file_line.size();
}


