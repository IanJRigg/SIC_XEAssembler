#include <iostream>
#include <fstream>
#include <vector>
#include "file_parser.h"
#include "file_parser_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"

#define FULL_SIZE 0

sicxe_asm::sicxe_asm() {
	symtab symbol_table();
	opcodetab opcode_table();
	lines.reserve(500);
	row_num = 0;
}

void sicxe_asm::first_pass(string filename) {
	in_filename = filename;
	file_parser parser(in_filename);
	parser.readfile();
	string location, label, operand, start_add;
	int location_counter = 0;
	int prog_len = 0;
	string opcode = parser.get_token(row_num, 1);
	
	if(opcode == 'START') {
		operand = parser.get_token(row_num, 2);
		location_counter = string_to_int(operand.substr(1, operand.size()));
		location = int_to_hex(location_counter);
		start_add = location;
		store_line(location, parser.get_token(row_num, 0),
			parser.get_token(row_num, 1), operand);
		row_num++;
	} 
	
	label = parser.get_token(row_num, 0);
	opcode = parser.get_token(row_num, 1);
	operand = parser.get_token(row_num, 2);
	while(opcode != 'END') {
		if(opcode != " ") {
			if(label != " ") {
				if(symbol_table.in_symtab(label)) {
					//throw exception
				} else {
					symbol_table.insert_symbol(label,
						location_counter, " ");
				}
				
				if(opcode_table.opcode_exists(opcode)) {
					location_counter += 3;
				} else {
					process_directives(opcode, operand);
				}
			}
		}
		location = int_to_hex(location_counter);
		store_line(location, parser.get_token(row_num, 0),
			parser.get_token(row_num, 1), operand);
		
		row_num++;
		label = parser.get_token(row_num, 0);
		opcode = parser.get_token(row_num, 1);
		operand = parser.get_token(row_num, 2);
	}
	location = int_to_hex(location_counter);
	store_line(location, label, opcode, operand);
	prog_len = hex_to_int(location_counter) - hex_to_int(start_add);
}

starting_address = int_location_counter;

    prog_len = int_location_counter - starting_address;
    print_file();
    cout << int_location_counter << endl;
    cout << starting_address << endl;
    cout << "Program length: " << prog_len << endl;

    file_parser parser(in_filename);
    try{
    parser.read_file();
    }catch(file_parse_exception ex){
        throw ex.getMessage();
    }
    string opcode, location;
    // Account for EQU statments
    while((opcode = parser.get_token(row_num, 1)) != "start"){
         if(opcode.compare(" ") != 0)
              throw_error("Command listed before program initialization");
         location = int_to_hex(location_counter);
         store_line(location, parser.get_token(row_num, 0), opcode, parser.get_token(row_num, 2));
         row_num++;
    }
    location = parser.get_token(row_num, 2); 
    if(location.find("$") != location.size())
        location_counter = hex_to_int(location.substr(1, location.size()));
    else
        location_counter = hex_to_int(location);
    location = int_to_hex(location_counter);
    store_line(location, parser.get_token(row_num, 0), parser.get_token(row_num, 1), parser.get_token(row_num, 2));
    
    row_num++;
    string label;
    // Account for labels at the beginning
    int opcode_size;
    
    //Verify if this part is valid code!!!
    unsigned int file_size = (unsigned int) parser.size();
    while(row_num < file_size){ // Check for "EOF"
        opcode = parser.get_token(row_num, 1);
        label = parser.get_token(row_num, 0);
        string operand = parser.get_token(row_num,2);
        if(opcode.compare(" ") == 0){
           // if labels are kept here, insert the label into the symtab
           store_line(int_to_hex(location_counter), label, opcode, operand);
           row_num++;
           continue;
        }
        if(label.compare(" ") != 0){
            symbol_table.insert_symbol(label, int_to_hex(location_counter),"");
        }
        string errorflag;
        try{
            opcode_size = opcode_table.get_instruction_size(opcode);
        } catch(opcode_error_exception ex){
            opcode_size = -1; //Why is this set to -1????
            errorflag=ex.getMessage();
        }
        
        if(opcode_size > 0){
            location_counter += opcode_size;
        }
        else{
            location_counter += process_directives(opcode,operand,errorflag);
        }
        store_line(int_to_hex(location_counter), label, opcode, parser.get_token(row_num, 2));
    
        
        
        
        row_num++;
    }
    print_file();
}

					
	
	
		
