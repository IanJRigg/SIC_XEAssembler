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
    in_filename = filename;
    symtab symbol_table();
    opcodetab opcode_table();
    lines.reserve(500);
    row_num = 0;
    location_counter = 0;
    base = 0;
    starting_address = 0;
}

sicxe_asm::~sicxe_asm(){}

void sicxe_asm::first_pass(){
	//in_filename = filename;
	file_parser parser(in_filename);
	parser.read_file();
	string location, label, operand, start_add;
	int location_counter = 0;
	int prog_len = 0;
	string opcode = parser.get_token(row_num, 1);
	string errorflag;
	int opcode_size;
	
	if(opcode.compare("START") == 0) {
		operand = parser.get_token(row_num, 2);
		location_counter = string_to_int(operand.substr(1, operand.size()));
		location = int_to_hex(location_counter);
		start_add = location;
		store_line(location, parser.get_token(row_num, 0),
			parser.get_token(row_num, 1), operand);
		row_num++;
	} 
	
	label = parser.get_token(row_num, 0);
	opcode = parser.get_token(row_num, 1);
	operand = parser.get_token(row_num, 2);
	while(opcode.compare("END") != 0) {
		if(opcode != " ") {
			if(label != " ") {
				try {
					symbol_table.insert_symbol(label,
						int_to_hex(location_counter), "R");
				} catch (symtab_exception ex) {
					throw ex.getMessage();
				}
			
        			try{
            				opcode_size = opcode_table.get_instruction_size(opcode);
        			} catch(opcode_error_exception ex){
            				opcode_size = -1; //Why is this set to -1????
            				errorflag=ex.getMessage();
        			}
        
        			if(opcode_size > 0){
            				location_counter += opcode_size;
        			}
        			else{
            				location_counter += process_directives(opcode,operand,errorflag);
        			}
			}
		}
		location = int_to_hex(location_counter);
		store_line(location, parser.get_token(row_num, 0),
			parser.get_token(row_num, 1), operand);
		
		row_num++;
		label = parser.get_token(row_num, 0);
		opcode = parser.get_token(row_num, 1);
		operand = parser.get_token(row_num, 2);
	}
	location = int_to_hex(location_counter);
	store_line(location, label, opcode, operand);
	prog_len = location_counter - hex_to_int(start_add);
}

 //Creates and throws a file_parse_exception
 //THIS CODE IS BAD!!!! We need a sicxe exception class!!
void sicxe_asm::throw_error(string error){
    ss_error<<"at line: "<<row_num + 1 << ", " << error;
    throw symtab_exception(ss_error.str());
}

//Converts a string to an int
int sicxe_asm::string_to_int(string s){
	istringstream instr(s);
	int n;
	instr >> n;
	return n;
}

// Converts a decimal integer to a hexadecimal string.
string sicxe_asm::int_to_hex(int num){
    stream << setw(5) << setfill('0') << hex << num;
    string tmp = to_uppercase(stream.str());
    stream.str("");
    return tmp;
}

//Function to convert decimal string to an integer
int sicxe_asm::dec_to_int(string s){
    int value;
    sscanf(s.c_str(),"%d",&value);
    return value;
}

void sicxe_asm::store_line(string address, string label, string opcode, string operand){
    prog_listing line;
    stream <<row_num + 1;
    line.line_number = stream.str();
    stream.str(""); //Clearing the stream
    line.address = address;
    line.label = label;
    line.opcode = opcode;
    line.operand = operand;
    lines.push_back(line);
}

void sicxe_asm::print_file() {
//prints the input file in proper format
    for(unsigned int i = 0; i <row_num; i++) {
        //Somewhere in code line_number is converted to HEX!
        cout << i+1/*lines.at(i).line_number*/ << "\t";
        cout << lines.at(i).address << "\t";
        cout << lines.at(i).label << "\t";
        cout << lines.at(i).opcode << "\t";
        cout << lines.at(i).operand << endl;
    }
}

bool sicxe_asm::is_hex(string s){
  int length = s.length();
  char * hex_string = new char(length);
  strcpy(hex_string, s.c_str());
  for(int i = 0; i < length; i++){
      if(!isxdigit(hex_string[i])) return false;  
  }
  return true;
}
int sicxe_asm::hex_to_int(string s){
     int value;
     sscanf(s.c_str(),"%x",&value);
     return value;
}

//Function to convert string to upper case
string sicxe_asm::to_uppercase(string s){
    transform(s.begin(),s.end(),s.begin(),::toupper);
    return s;
}

int sicxe_asm::character_count(string s){
    int count=0;
    char c;
    /**Setting i = 1 to skip c or x and only count characters
     * between the quotes*/
    for(unsigned int i = 1;i<s.size();i++){
        c = s[i];
        if(i==1 && c != '\''){
            //Set this to an exception throw of type sicxe
            cout<<"Count format is bad"<<endl;
            exit(1);
        } 
        if(c=='\'')continue;
        count++;
    }
    return count;
}

//Change this to riggins style of call!!!!!!!!!!
string sicxe_asm::validate_address(string address){
    unsigned int length = address.length();
    switch(length){
        case 1:
            return "0000" + address;
        case 2:
            return "000" + address;
        case 3:
            return "00" + address;
        case 4:
            return "0" + address;
        case 5:
            return address;
        default:
            throw_error("Starting address is not between 1 and 5 digits");
            return "";
    }
}

//TODO verify error checking!!!!!
int sicxe_asm::count_byte_operand(string operand){
    int count = 0;
    string c = to_uppercase(operand.substr(0,1));
    if(c.compare("C")==0)
        count += character_count(operand);
    if (c.compare("X")==0){
        int tmp = character_count(operand);
        if(tmp%2 !=0){
            //fix this throw type!!!!
            throw_error("Invalid BYTE operand");
        }
        count += tmp;
    }
    return count;
}

//Need to add error checking to this function!!!TODO
int sicxe_asm::count_resw_operand(string operand){
    int count = 0;
    //set constant variable to word size 3 and replace TODO
    count += dec_to_int(operand)*3;
    return count;
}

//TODO add error checking!!!!
int sicxe_asm::count_resb_operand(string operand){
    int count = 0;
    count += dec_to_int(operand);
    return count;
}

/*Takes an opcode and operand as parameters
 *Processes any preprocessor directives
 *Returns value to be added to address
*/
int sicxe_asm::process_directives(string opcode, string operand, string error){
    int count = 0;
    string tmp = to_uppercase(opcode);
    if(tmp.compare("BYTE") ==0){
        count += count_byte_operand(operand);
        return count;        
    }
    else if(tmp.compare("RESW")==0){
        count += count_resw_operand(operand);
        return count;
    }
    else if(tmp.compare("WORD")==0){
        count += 3;
        return count;
    }
    else if(tmp.compare("RESB")==0){
        count += count_resb_operand(operand);
        return count;
    }
    else if(tmp.compare("EQU")==0){
        //process EQU here
        return count;
    }
    else if(tmp.compare("BASE")==0){
        //base=string_to_int(operand);
        return count;
    }
    else if(tmp.compare("NOBASE")==0){
        base = 0;
        return count;
    }
    /*//GET RID OF THIS WHEN REFACTORED!!!!
    else if(tmp.compare("END")==0){
        return count;
    }*/
    else{   
        throw error;        
    }
    
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
    catch(string error){
        cout<< "Error in sicxe_asm: "<<source_file_name<<" \n"<<error<<endl;
    }
    return 0;
}

