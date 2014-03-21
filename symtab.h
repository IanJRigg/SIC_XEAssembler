/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog3 : symtab.h
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
		/** Constructor*/
		symtab();

		/** Destructor */
		~symtab();
                
                /**Returns the key value*/
		string get_value(string);
                
                /**Inserts a symbol and its value into the table*/
		void insert_symbol(string, string, string);
                
                /**Checks if flag set for relative address or absolute value
                 * Relative = return 1
                 * Absolute = return 0  
                 */
                int check_flag(string);
                 
                /**Sets the flag if value is relative address or absolute value
                  * Relative = 1
                  * Absolute = 0
                  */
                void set_flag(string);
            

	private:
		map<string, pair<string,string> > symbol_table;
                                                
                /**Checks if value exists in symbol table*/
		bool in_symtab(string);
		
};
#endif
