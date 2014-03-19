/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    file_parser.cc
    CS530, Spring 2014
*/
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(){}

symtab::~symtab() {}

bool symtab::already_in_symtab(string key){
    return (symbol_table.find(key) != symbol_table.end());
}

string symtab::get_value(string key){
    if(!already_in_symtab(key))
      return ""; // Replace this with a thrown error
    return (symbol_table.find(key)->second);
}

void symtab::insert(string key, string value){
    symbol_table.insert(make_pair(key, value));
}
