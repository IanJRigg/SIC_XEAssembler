/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog1 : sicxe_asm.cpp
    CS530, Spring 2014
    Team Florida
*/

#include "sicxe_asm.h"

using namespace std;

sicxe_asm::sicxe_asm(string filename){
  source_file_name = filename;
  symtab symbol_table();
}

sicxe_asm::~sicxe_asm(){}

void sicxe_asm::first_pass(){
	file_parser parser(source_file_name);
	parser.read_file();
 
    string label, opcode;
    int i = 0;
    while((opcode = parser.get_token(i, 1)) != "start"){
      if(opcode.compare(" ") != 0)
        throw symtab_exception("Command on line:  listed before program initialization");
      i++;
      cout << opcode << endl;
    }
    /*
    location_counter = parser.get_token(i,2); // Convert to integer to begin operations
    start_address = location_counter; // integer location_counter, need to place a third variable

    while((opcode = get_token(++i, 1)) != 'EOF'){
      if(nextLine == ' ')
        listings.at(i).address = location_counter;
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

int main(int argc, char *argv[]){
  if(argc != 2){
    cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
  }
  string source_file_name = argv[1];
  sicxe_asm assembler(source_file_name);
  try{
    assembler.first_pass();
  }
  catch(symtab_exception ex){
    cout << ex.getMessage() << endl;
  }
  return 0;
}
