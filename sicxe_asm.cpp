/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog1 : file_parser.h
    CS530, Spring 2014
    Team Florida
*/

#include "sicxe_asm.h"

using namespace std;

int main(int argc, char *argv[]){
	if(argc != 2){
		cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
	}
	string filename = argv[1];
	symtab symbol_table(filename);
	symbol_table.read_file();
	symbol_table.create_symbol_table();
	symbol_table.assign_addresses();
	symbol_table.write_listing_file();

	return 0;
}
