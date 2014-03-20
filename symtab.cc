/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    symtab.cc
    CS530, Spring 2014
*/
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(){}

symtab::~symtab() {}

bool symtab::in_symtab(string key){
    return (symbol_table.find(key) != symbol_table.end());
}

/**To be defined in project 4*/
int symtab::check_flag(string key){return 0;}

/**To be defined in project 4*/
void symtab::set_flag(string key){}

string symtab::get_value(string key){
    if(!in_symtab(key))
      throw symtab_exception("Symbol does not exist in table");
    return (symbol_table.find(key)->second.first);
}

void symtab::insert_symbol(string key, string value, string flag){
    if(in_symtab(key))
        throw symtab_exception("Unable to insert, key exists in symtab");
    symbol_table[key] = pair<string,string> (value,flag);
}



