/*
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    opcodetab.cc
    CS530, Spring 2014
*/

opcodetab::opcodetab(){
    //all the opcodes I could find in back of book 58 total
    string [] codes = {"ADD","ADDF","ADDR","AND","CLEAR","COMP","COMPF","COMPR","DIV","DIVF","DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB","LDA","LDB","LDCH","LDF","LDL","LDS","LDT","LDX","LPS","MULF","MULR","NORM","OR","RD","RMO","RSUB","SHIFTL","SHIFTR","SIO","SSK","STA","STB","STCH","STF","STI","STL","STS","STSW","STT","STX","SUB","SUBF","SUBR","SVC","TD","TIO","TIX","TIXR","WD"}
    
    //don't think this is accurate probably need some sort of for loop
    m[codes[i]]= values[i]
}

opcodetab::get_machine_code();

opcodetab::get_instruction_size();