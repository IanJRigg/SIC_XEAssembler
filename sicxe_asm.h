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
#include <cstdlib>
#include <iomanip>


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
                string in_filename;
		stringstream ss_error;
                stringstream stream;
		vector<prog_listing> lines;
		int row_num;
                int location_counter;
                int base;
                int starting_address;
		symtab symbol_table;
		opcodetab opcode_table;

		void store_line(string, string, string, string);
		void print_file();
		bool is_hex(string);
                
                //Conversion functions
		int hex_to_int(string);
                string int_to_hex(int);
                int dec_to_int(string);
                string format_8(string);
                
                string to_uppercase(string);
                int character_count(string);
                int process_directives(string,string);
                int count_byte_operand(string);
                int count_resw_operand(string);
                int count_resb_operand(string operand);
                
		string validate_address(string);
                int verify_start_location_value(string);
		string increment_location_counter(string, unsigned int);
                string error_format(string);
		
};
#endif
