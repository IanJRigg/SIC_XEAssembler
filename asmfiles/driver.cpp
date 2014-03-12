#include <iostream>
#include <fstream>
#include <vector>
#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"

//change to 1 when using source8.asm
#define FULL_SIZE 0
using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
    }
    string filename = argv[1];
    try {
	    file_parser parser(filename);
	    parser.read_file();
	    parser.print_file();
	    opcodetab table;
	    if(!FULL_SIZE){
			cout << "Token at 2,0 is: " << parser.get_token(2,0) << endl;    
			cout << "Token at 2,1 is: " << parser.get_token(2,1) << endl;
			cout << "Token at 2,2 is: " << parser.get_token(2,2) << endl;
			cout << "Token at 2,3 is: " << parser.get_token(2,3) << endl;
			    
			cout << "Token at 15,0 is: " << parser.get_token(15,0) << endl;    
			cout << "Token at 15,1 is: " << parser.get_token(15,1) << endl;
			cout << "Token at 15,2 is: " << parser.get_token(15,2) << endl;
			cout << "Token at 15,3 is: " << parser.get_token(15,3) << endl;   
			    
			string code = parser.get_token(12,1);
			string mac_code = table.get_machine_code(code);
			cout << code <<" hex is " << mac_code <<" ";
			int format_size = table.get_instruction_size(code);
			cout << code <<" size is " << format_size << endl;
		}
    

	    /*if(FULL_SIZE){
	    	for(int i=0;i<125;i++){
	    		try{
	   				string code2 = parser.get_token(i,1);
	    			string mac_code2 = table.get_machine_code(code2);
	    			cout << code2 <<" hex is " << mac_code2<<" ";
	    			int format_size = table.get_instruction_size(code2);
	    			cout << code2 <<" size is " << format_size << endl; 
	    		}
	    		catch(opcode_error_exception exchk){
	    		    cout<<"**Sorry, error "<< exchk.getMessage()<<endl;
	    		}
	    	}
	    } */
    }
    catch(file_parse_exception excpt) {
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
    catch(opcode_error_exception excpt) {
    	cout << "***Sorry, exception " << excpt.getMessage() << endl;
    }
}
