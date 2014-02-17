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
	if(in.open(in_file_name, ios::in) != 0) {
		//throw file_parse_exception
	}
	while (!in.eof()) {
		getline(in, parsed_line);
		file_line.pushback(parse_tokens(line, parsed_line);
	}
	in.close();
}

file_parser::parse_tokens(line fline, string parsed_line_string) {
	if(parsed_line_string[0] == ' ') //check syntax here for checking column 0
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


