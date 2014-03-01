/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    Prog2: opcodedriver.cpp
    CS530 Spring 2014
    Team Florida
*/ 

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

int main(int argc, char *argv[]) {
    try {
    std::string codes[] = { "ADD", "AdD","aDd","adD","Add","aDD","ADd", "+ADD","+aDd","+adD","+Add", "ADDF", "+ADDF", "ADDR", "AND", "+AND", "CLEAR", "COMP", "+COMP", "COMPF", 
				"+COMPF","COMPR","DIV","+DIV", "DIVF","+DIVF", "DIVR", "FIX", "FLOAT", "HIO", 
				"J","+J", "JEQ","+JEQ", "JGT","+JGT", "JLT","+JLT", "JSUB","+JSUB", "LDA", "+LDA", "LDB","+LDB", 
				"LDCH","+LDCH", "LDF","+LDF", "LDL","+LDL", "LDS","+LDS", "LDT","+LDT", "LDX","+LDX", "LPS", "+LPS",
				"MUL","+MUL", "MULF","+MULF", "MULR", "NORM", "OR","+OR", "RD","+RD", "RMO", "RSUB","+RSUB", 
				"SHIFTL", "SHIFTR", "SIO", "SSK","+SSK", "STA","+STA", "STB","+STB", "STCH", "+STCH",
				"STF","+STF", "STI","+STI", "STL","+STL", "STS","+STS", "STSW","+STSW", "STT","+STT", "STX", "+STX",
				"SUB","+SUB", "SUBF","+SUBF", "SUBR", "SVC", "TD","+TD", "TIO", "TIX","+TIX", 
				"TIXR", "WD","+WD" };
                                
     std::string invalid_format_four[] = {"+ADDR","+CLEAR","+COMPR","+DIVR","+FIX","+FLOAT",
                                "+HIO","+MULR","+NORM","+RMO","+SHIFTL","+SHIFTR","+SIO","+SUBR",
                                "+SVC","+TIO","+TIXR"};
                                
    opcodetab opcode;
    string machinecode; 
    int instructsize;
    for(unsigned int i=0;i<(sizeof(codes)/sizeof(*codes)); i++){
        machinecode = opcode.get_machine_code(codes[i]);
        instructsize = opcode.get_instruction_size(codes[i]);
    
        cout<<"For opcode "<<codes[i]<<": the machine code is: ";
        cout<<machinecode<<" and the size is: "<<instructsize<<endl;
        if(codes[i][0]=='+'){
            if(instructsize != 4){
                cout<<"Size failed to increment for opcode format 4"<<endl;
            }
        }       
    }
    
    unsigned int i=0;
    while(i<INVALID_FORMAT_FOUR_NUM){
    
        cout<<"Testing bad machine code value: "<<invalid_format_four[i]<<endl;
        try{
            machinecode = opcode.get_machine_code(invalid_format_four[i]);
        }
        catch(opcode_error_exception caught){
            cout<<"**Caught bad format "<<caught.get_message()<<endl;
        }
        
        cout<<"Testing bad instructions size value: "<<invalid_format_four[i]<<endl;
        
        try{    
        instructsize = opcode.get_instruction_size(invalid_format_four[i]);
        }
        catch(opcode_error_exception caught){
            cout<<"**Caught bad format "<<caught.get_message()<<endl;
        }
        i++;
    }
    }
    catch(opcode_error_exception excpt) {
        cout << "**Sorry, error " << excpt.get_message() << endl;
    }
    
    
}
