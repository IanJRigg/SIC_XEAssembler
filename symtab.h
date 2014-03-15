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

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
		string file_to_read;
		file_parser parser(string);
		vector<prog_listing> listings;
		int size;
};
#endif