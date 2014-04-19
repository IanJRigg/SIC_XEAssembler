/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Prog 3: symtab.cc
    CS530, Spring 2014
*/
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(){}

symtab::~symtab() {}

bool symtab::in_symtab(string key){
    symbol_table_iterator=symbol_table.find(to_uppercase(key));
    if(symbol_table_iterator == symbol_table.end()){
        return false;
    }
    return true;
}

/**To be defined in project 4*/
string symtab::check_flag(string key){
    return (symbol_table.find(to_uppercase(key))->second.second);
}

/**To be defined in project 4*/
void symtab::set_flag(string key, string flag){
    symbol_table.find(to_uppercase(key))->second.second = flag;
}

string symtab::get_value(string key){
    if(!in_symtab(to_uppercase(key)))
      throw symtab_exception("Symbol does not exist in table");
    return (symbol_table.find(to_uppercase(key))->second.first);
}

void symtab::insert_symbol(string key, string value, string flag){
    if(in_symtab(to_uppercase(key))){
        throw symtab_exception("Unable to insert, key exists in symtab::Key: "+key);
    }  
    symbol_table[to_uppercase(key)] = pair<string,string> (value,flag);
}

string symtab::to_uppercase(string s){
    transform(s.begin(),s.end(),s.begin(),::toupper);
    return s;
}



