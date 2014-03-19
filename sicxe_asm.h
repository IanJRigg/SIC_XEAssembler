/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog1 : sicxe_asm.h
    CS530, Spring 2014
    Team Florida
*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

#include <iostream>
#include <string>


class sicxe_asm{
	public: 
		sicxe_asm(string);

		~sicxe_asm();

		void first_pass();

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
		string source_file_name;
		stringstream ss_error;
		vector<prog_listing> lines;
		unsigned int size;
		symtab symbol_table;
		opcodetab opcode_table;

		void throw_error(string);
		void write_to_lines(string, string, string, string);
		void print_file();
		string validate_address(string);
		string increment_location_counter(string, unsigned int);
		string itohs(string);
};
#endif