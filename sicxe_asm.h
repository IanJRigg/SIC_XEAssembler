/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog3 : sicxe_asm.h
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
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>


#define WORD_SIZE 3
//Hex values of flags
#define n_hex 0x020000
#define i_hex 0x010000
#define x_hex 0x008000
#define b_hex 0x004000
#define p_hex 0x002000
#define e_hex 0x001000

class sicxe_asm{
    public: 
        sicxe_asm(string);

        ~sicxe_asm();

	void first_pass();
        
        void second_pass();

    private:
	struct prog_listing{
		string line_number;
		string address;
		string label;
		string opcode;
		string operand;
                string m_code;
		prog_listing():  line_number(" "),
				 address(" "),
				 label(" "),
				 opcode(" "),
				 operand(" "),
                                 m_code(" "){}
	};
        string lis_filename;
        string in_filename;
        string start_name;
        string address;
        string opcode;
        string operand;
        string label;
        string m_code;
        string base;
        
        //Registers
        string a_reg;
        string b_reg;
        string x_reg;
        string s_reg;
        string t_reg;
        string l_reg;
        string pc_reg;
        string sw_reg;
        
        //Flags
        bool n_bit;
        bool i_bit;
        bool x_bit;
        bool b_bit;
        bool p_bit;
        bool e_bit;
        
        //Base Check Flag
        bool base_set;
        
	stringstream ss_error;
        stringstream stream;
	vector<prog_listing> lines;
	int row_num;
        int int_location_counter;
        int file_size;
        int starting_address;
        int prog_len;
        int op_size;
        int target_location;        
        
	symtab symbol_table;
	opcodetab opcode_table;

	//Storage and Output Functions	
	/*****************************************************************************
 	*Method: store_line                                                          *
	*Parameters: none                                                            *
 	*Purpose:  stores the line#, address, label. opcode and operand into a struct*
 	*Returns: pushes struct into a vector                                        *
	*Error handling: none                                                        *
 	******************************************************************************/ 
	void store_line();
	
	/***************************************************************************
 	*Method: print_file();                                                     *
	*Parameters:none                                                           *
 	*Purpose:  prints listing file to command line                             *
 	*Returns:  none                                                            *
	*Error handling: none                                                      *
 	****************************************************************************/ 
	void print_file();
	
	/***************************************************************************
 	*Method: write_file();                                                     *
	*Parameters:none                                                           *
 	*Purpose:  writes the listing file to a .lis file                          *
 	*Returns:                                                                  *
	*Error handling: none                                                      *
 	****************************************************************************/ 
        void write_file();
                
        //Validation functions
				
	/***************************************************************************
 	*Method: is_hex(string);                                                   *
	*Parameters: string                                                        *
 	*Purpose:  verifies if given string is a hexidecimal value                 *
 	*Returns:  true or false                                                   *
	*Error handling: handled by isxdigit()                                     *
 	****************************************************************************/ 
	bool is_hex(string);
			
	/***************************************************************************
 	*Method: is_num(string);                                                   *
	*Parameters: string                                                        *
 	*Purpose:  verifies if given string is a decimal value                     *
 	*Returns: true or false                                                    *
	*Error handling:  isdigit()                                                *
 	****************************************************************************/ 
        bool is_num(string);
				
	/***************************************************************************
 	*Method: verify_start_location_value(string);                              *
	*Parameters: string                                                        *
 	*Purpose:  Processes incomming starting address value                      *  
	*If value is designated as hex it returns the hex equivalent value in int  *
 	*Returns: interger value of variable                                       *
	*Error handling: Catches invalid starting address values                   *
 	****************************************************************************/ 
        int verify_start_location_value(string);
		
	/***************************************************************************
 	*Method: bool string_compare(string,string);                               *
	*Parameters: string                                                        *
 	*Purpose: compares two given string values and changes to uppercase        *
 	*Returns: true or false                                                    *
	*Error handling:  none                                                     *
 	****************************************************************************/ 
        bool string_compare(string,string);
                
        //Conversion functions
		
	/************************************************************
 	*Method: hex_to_int(string s);                              *
	*Parameters: string                                         *
 	*Purpose:Converts hexadecimal string to an integer          *
 	*Returns:integer                                            *
        *Error handling: none                                       *
 	*************************************************************/
	int hex_to_int(string);
	
        /************************************************************
 	*Method: int_to_hex(int num);                               *
	*Parameters: int                                            *
 	*Purpose:Converts a decimal integer to a hexadecimal string *
 	*Returns:value as a hexadecimal string                      *
        *Error handling: none                                       *
 	*************************************************************/		
        string int_to_hex(int);
		
	/***************************************************************************
 	*Method: dec_to_int(string s);                                             *
	*Parameters: string                                                        *
 	*Purpose: convert decimal string to an integer                             *
 	*Returns:integer                                                           *
	*Error handling: If parameter is invalid for conversion it throws an error *
 	****************************************************************************/
        int dec_to_int(string);
		
	/***************************************************************************
 	*Method: format_8(string x);                                               *
	*Parameters: string                                                        *
 	*Purpose: formats a line to fill 8 spaces                                  *
 	*Returns: 8 character string                                               *
	*Error handling: none                                                      *
 	****************************************************************************/
        string format_8(string);
		
	/***************************************************************************
 	*Method: format_15(string x);                                              *
	*Parameters: string                                                        *
 	*Purpose: formats a line to fill 15 spaces                                 *
 	*Returns: 15 character string                                              *
	*Error handling: none                                                      *
 	****************************************************************************/
        string format_15(string);
		
	/***************************************************************************
 	*Method: string_to_int(string s);                                          *
	*Parameters: string                                                        *
 	*Purpose: converts string variables into integers                          *
 	*Returns: integer                                                          *
	*Error handling: none                                                      *
 	****************************************************************************/
        int string_to_int(string);
		
	/***************************************************************************
 	*Method: to_uppercase(string s);                                           *
	*Parameters: string                                                        *
 	*Purpose: converts string to uppercase                                     *
 	*Returns: returns uppercase string                                         *
	*Error handling: none                                                      *
 	****************************************************************************/                
        string to_uppercase(string);
                                
        /***************************************************************************
 	*Method: character_count(string s);                                        *
	*Parameters: string                                                        *
 	*Purpose: counts the number of characters for a byte operand               *
 	*Returns: integer                                                          *
	*Error handling:  invalid operand formatting                               *
 	****************************************************************************/   
	int character_count(string);
		
	/****************************************************************************
 	*Method: process_directive()                                                *
	*Parameters:none                                                            *
 	*Purpose: processes any preprocessor directives                             *
 	*Returns:Integer value to be added to address                               *
	*Error handling:checks if start is called multiple times or end before start*
 	*****************************************************************************/   
        int process_directives();

	/***************************************************************************
 	*Method: process_base();                                                   *
	*Parameters: none                                                          *
 	*Purpose: Handles Base directive commands                                  *
 	*Returns: none                                                             *
	*Error handling: none                                                      *
 	****************************************************************************/   
        void process_base();
				
	/***************************************************************************
 	*Method: process_equ();                                                    *
	*Parameters: none                                                          *
 	*Purpose: handles equ directive commands                                   *
 	*Returns:Value to be added to address                                      *
	*Error handling: symtab_exception                                          *
 	****************************************************************************/   
        void process_equ();
				 
	/***************************************************************************
 	*Method: count_byte_operand(string);                                       *
	*Parameters:none                                                           *
 	*Purpose: counts the byte operand chacters based on c or x conditions      *
 	*Returns:returns count of bytes                                            *
	*Error handling:  if formatted improperly will throw an error              *
 	****************************************************************************/ 
        int count_byte_operand();
				
	/***************************************************************************
 	*Method: count_resw_operand(string);                                       *
	*Parameters: none                                                          *
 	*Purpose: counts the number of words to reserve                            *
 	*Returns:returns number of words                                           *
	*Error handling:   perfomed by dec_to_int                                  *
 	****************************************************************************/ 
        int count_resw_operand();
				
	/***************************************************************************
 	*Method: count_resb_operand(string);                                       *
	*Parameters: none                                                          *
 	*Purpose: counts the number of bytes to reserve                            *
 	*Returns:returns number of bytes                                           *
	*Error handling:  perfomed by dec_to_int                                   *
 	****************************************************************************/ 
        int count_resb_operand();
				             
	/***************************************************************************
 	*Method: error_format(string);                                             *
	*Parameters: string                                                        *
 	*Purpose:  Takes in a string message and returns a line # and a message    *
 	*Returns: error message                                                    *
	*Error handling: none                                                      *
 	****************************************************************************/ 
        string error_format(string);
				
	/***************************************************************************
 	*Method: assemble();                                                       *
	*Parameters: none                                                          *
 	*Purpose:  Processes pass one and pass two of the assembler                *
 	*Returns: error message                                                    *
	*Error handling: Catches any errors thrown by first_pass()                 *
 	****************************************************************************/ 
	void assemble();
        
       /************************************************************
        *Method: check_addr_mode(string)                           *
        *Parameters: operand                                       *
        *Purpose:Validate operand first character for #, @ or none *
        *Returns: 1 if #, 2 if @ , 3 if $, 0 if none               *
        *Error Handling: None                                      *
        ************************************************************/ 
        int check_addr_mode(string);
	
	/***************************************************************************
	*Method: validate_tf_operand(string);					   *
	*Parameters: operand string						   *
	*Purpose: To check the validity of the operands				   *
	*Returns: True, if operand is valid, false otherwise			   *
	*Error handling: none							   *
	****************************************************************************/
	string validate_tf_operand(string);
        
        /***************************************************************************
	*Method: is_process_directive(string);					   *
	*Parameters: string opcode						   *
	*Purpose: To check if opcode is a processor directive	                   *
	*Returns: True, if opcode is directive, false otherwise			   *
	*Error handling: none							   *
	****************************************************************************/
        bool is_process_directive(string);
        
        /***************************************************************************
	*Method: validate_registers(string);					   *
	*Parameters: string operand						   *
	*Purpose: To check if registers are valid          		           *
	*Returns: True, if registers are valid, false otherwise			   *
	*Error handling: none							   *
	****************************************************************************/ 
        bool validate_registers(string);
        
        /***************************************************************************
	*Method: parse_operand(string, string&, string&);			   *
	*Parameters: string operand, string reference1, string reference2          *
	*Purpose: parses operand and passes changes to op1/op2 as references       *
	*Returns: None                                   			   *
	*Error handling: none							   *
	****************************************************************************/
        void parse_operand(string, string&, string&);
        
        /***************************************************************************
	*Method: process_forward_ref(string)      			           *
	*Parameters: string operand reference                                      *
	*Purpose: checks current operand against symbol table and replaces it      *
	*Returns: None                                   			   *
	*Error handling: none							   *
	****************************************************************************/        
        void process_forward_ref(string&);
        
        /***************************************************************************
	*Method: check_registers(string)          			           *
	*Parameters: string register                                               *
	*Purpose: checks current register and returns register value               *
	*Returns: value of register, -1 if invalid register    			   *
	*Error handling: none							   *
	****************************************************************************/
        string check_registers(string);
        
        /***************************************************************************
	*Method: process_format_one(string)          			           *
	*Parameters: string opcode                                                 *
	*Purpose: Checks if instruction has operand, if not sets m_code            *
	*Returns: None                                  			   *
	*Error handling: Throws if opcode has an operand         		   *
	****************************************************************************/
        void process_format_one(string);
        
        /***************************************************************************
	*Method: validate_offset_size(string)          			           *
	*Parameters: string offset                                                 *
	*Purpose: Checks if given offset is within the bounds of the format size   *
	*Returns: True if it validates, false otherwise       			   *
	*Error handling: None                                    		   *
	****************************************************************************/	
	bool validate_offset_size(string);
        
        /***************************************************************************
	*Method: process_format_three_offset(string)  			           *
	*Parameters: string operand                                                *
	*Purpose: Calculates operand offset for given format 3 instruction         *
	*Returns: Int value of offset                         			   *
	*Error handling: Throws if operand is formatted incorrectly or invalid     *
	****************************************************************************/
        int process_format_three_offset(string);        
        
        /***************************************************************************
	*Method: process_format_four_offset(string)  			           *
	*Parameters: string operand                                                *
	*Purpose: Calculates operand offset for given format 4 instruction         *
	*Returns: Int value of operand based on addressing mode type     	   *
	*Error handling: Throws if operand is formatted incorrectly or invalid     *
	****************************************************************************/
        int process_format_four_offset(string);
		
};
#endif
