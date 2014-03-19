/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog1 : file_parser.h
    CS530, Spring 2014
    Team Florida
*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>
#include <map>

using namespace std;


class symtab{
	public:
		// Constructor
		symtab();

		// Destructor
		~symtab();

		bool already_in_symtab(string);

		string get_value(string);

		void insert(string, string);

	private:
		map<string, string> symbol_table;
		
};
#endif