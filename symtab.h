/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog1 : file_parser.h
    CS530, Spring 2014
    Team Florida
*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include "file_parser.h"
#include "opcodetab.h"
#include <string>

using namespace std;

class file_parser;

class symtab{
	public:
		// Constructor
		symtab(string);

		// Destructor
		~symtab();

		// Populate the symtab class' vector with the contents of file_parser's vector
		void read_file();

		void assign_addresses();

		void create_symbol_table();

		void write_listing_file();

	private:
		struct prog_listing{
			string line_number;
			string address;
			string label;
			string opcode;
			string operand;
			prog_listing():  line_number(" "),
					address(" "),
					label(" "),
					opcode(" "),
					operand(" "){}
		};
		file_parser parser;
		vector<prog_listing> listings;
		int size;
};
symtab::symtab(string filename):
  file_parser(filename)
  {}
#endif