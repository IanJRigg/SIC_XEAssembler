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
    file_parser::file_parser parser(filename);
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
	//address assembler directives
}
void symtab::assign_addresses(){
     /* 
    string label, opcode;
    while((opcode = get_token(i, 1)) != 'START'){
      if(nextLine != '')
        throw symtab_exception("Command on line:  listed before program initialization");
      i++;
    }
    location_counter = parser.get_token(i,2); // Convert to integer to begin operations
    start_address = location_counter; // integer location_counter, need to place a third variable

    while((opcode = get_token(++i, 1)) != 'EOF'){
      if(nextLine == ' ')
        continue;
      if((label = parser.get_token(i, 0)) != ' ')
        if(already_in_symtab(label))
          throw symtab_exception("Label on line:   already exists");
        insert into symtab, location counter is the value
        boolean opcode_found = lookup_opcode(opcode); // private method
        if(opcode_found){
          location_counter += size of command
        }
        else if(it's an assembler directive){
          handle the directive
        }
        else{
          throw symtab_exception("Uknown command on line: ");
        }
    } 
    */
}
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