/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    file_parser.cc
    CS530, Spring 2014
*/
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(string filename){
    file_parser parser(filename);
    parser.read_file();
    parser.print_file();
    size = parser.size();
	  listings.reserve(500);
}

symtab::~symtab() {}

void symtab::read_file(){
	//populate the vector with the data from the file_parser
  for(int i = 0; i < size; i++){
    listings.push_back(prog_listing());
    listings.at(i).line_number = string.to_string(i);
    listings.at(i).label = parser.get_token(i, 0);
    listings.at(i).opcode = parser.get_token(i, 1);
    listings.at(i).operand = parser.get_token(i, 2);
  }
}
void symtab::create_symbol_table(){
	//assign addresses
	//address assembler directives
}
void symtab::assign_addresses(){

}
void symtab::write_listing_file(){
  for(int i = 0; i < size; i++){
    cout << listings.at(i).opcode << endl;
  }
	/*				          **prog1.asm**
                        
  Line#     Address     Label        Opcode       Operand   
  =====     =======     =====        ======       =======
      1       00000
      2       00000     PROG1        START        $1000
      3       01000     FIRST        LDA          ALPHA
    */

}