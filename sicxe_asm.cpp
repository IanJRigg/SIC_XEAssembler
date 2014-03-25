/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    prog3 : sicxe_asm.cpp
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
    int_location_counter = 0;
    base = "-1";
    starting_address = 0;
    prog_len = 0;
    assemble();
}

sicxe_asm::~sicxe_asm(){}

/***********************************************************
*Called to begin the 2 pass assembler                      *
***********************************************************/
void sicxe_asm::assemble() {
	try {
		first_pass();
	} catch (string error) {
		cout << "Error in sicxe_asm::first_pass(): " 
		<< "\n" << error << endl;
		exit(1);
	}
}

/************************************************************
 *Insert comments here describing the first_pass() algorithm*
 ***********************************************************/
void sicxe_asm::first_pass(){
    file_parser parser(in_filename);
    try{
    parser.read_file();
    }catch(file_parse_exception ex){
        throw error_format(ex.getMessage());
    }    
    opcode = parser.get_token(row_num, 1); 
    string hex_location_counter;    
    /********************************************************************** 
     *Checks opcode for start command                                     *
     * if not found, verifies that opcode is valid prior to initialization*
     * stores information and proceeds to next line                       *
     **********************************************************************/
    while(to_uppercase(opcode).compare("START") != 0){
         hex_location_counter = int_to_hex(int_location_counter);
         store_line(hex_location_counter, " ", " ", " ");
         try{
         opcode = parser.get_token(++row_num, 1);
         }catch(file_parse_exception fex){
            throw error_format(fex.getMessage());
         }
    }    
    /* If opcode is start command then location is replaced with operand value
     * current address is stored on line
     */
    hex_location_counter = parser.get_token(row_num, 2);
    label = parser.get_token(row_num,0);
    operand = parser.get_token(row_num,2);
    start_name = to_uppercase(label);
    store_line(int_to_hex(int_location_counter), label, opcode, operand);
    
    /***************************************************** 
     *Checks if start is a hex declaration with $ symbol *
     * if not operand is taken for starting address value*
     *****************************************************/
    int_location_counter = verify_start_location_value(hex_location_counter);
    starting_address = int_location_counter;
    row_num++;
    int file_size = parser.size();
    while(row_num < file_size){         
        opcode = parser.get_token(row_num, 1);
        label = parser.get_token(row_num, 0);
        operand = parser.get_token(row_num,2);
        hex_location_counter = int_to_hex(int_location_counter);
        store_line(hex_location_counter, label, opcode, operand);
        if(opcode.compare(" ") == 0){
           row_num++;
           continue;
        }
        if(to_uppercase(opcode).compare("END")==0){
            if(operand.compare(" ")!=0){
                if(to_uppercase(operand).compare(to_uppercase(start_name))!=0){
                    throw error_format("Program end name does not match:(Operand: "+operand+") does not match start label (Label:"+start_name+")");
                }
            }              
            row_num++;
            break;
        }        
        if(label.compare(" ") != 0 && to_uppercase(opcode).compare("EQU")!=0){
            try{
            symbol_table.insert_symbol(label, hex_location_counter,"R");
            }catch(symtab_exception symex){
                throw error_format(symex.getMessage());
            }
        }
        int opcode_size =0;
        string errorflag="";
        try{
            opcode_size = opcode_table.get_instruction_size(opcode);
        } catch(opcode_error_exception ex){
            errorflag=ex.getMessage();            
        }
        if(errorflag.size()>1){
                opcode_size = process_directives(label,opcode,operand);
                if(opcode_size==-1){
                    throw error_format(errorflag);
                };
        }
        int_location_counter+=opcode_size;
        row_num++;
    }
    prog_len = int_location_counter - starting_address;
    print_file();
    write_file();
}

/*****************************************************
 *Converts a decimal integer to a hexadecimal string.*
 *****************************************************/
string sicxe_asm::int_to_hex(int num){
    stream<<setw(5)<<setfill('0')<<hex<<num;
    string tmp = to_uppercase(stream.str());
    stream.str("");
    return tmp;
}

/***********************************************************
 *Function to convert decimal string to an integer         *
 *If parameter is invalid for conversion it throws an error*
 ***********************************************************/
int sicxe_asm::dec_to_int(string s){
    if(!is_num(s)){
        throw error_format("Invalid decimal conversion candidate::"+s);
    }
    int value;    
    sscanf(s.c_str(),"%d",&value);
    return value;
}

/*************************************************************
 *Stores the address, label, opcode and operand into a struct*
 *Pushes the struct into a vector                            *
 ************************************************************/
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

/*********************************************
 *Prints the listing file to the command line*
 *********************************************/
void sicxe_asm::print_file() {
    cout<<setw(25)<<"**"<<in_filename.substr(0,in_filename.size()-4)<<"**"<<endl;
    cout<<setw(7)<<"Line#"<<setw(12)<<"Address"<<setw(10)<<"Label"<<setw(14)<<"Opcode";
    cout<<setw(14)<<"Opcode"<<endl;
    cout<<setw(7)<<"====="<<setw(12)<<"======="<<setw(10)<<"====="<<setw(14)<<"======";
    cout<<setw(14)<<"======="<<endl;
    for(int i = 0; i < row_num; i++) {
        cout << setw(7)<<i+1<< " ";        
        cout << setw(11)<<format_7(lines.at(i).address)<<" ";
        cout << setw(9)<<format_7(lines.at(i).label) << " ";
        cout << setw(13)<<format_7(lines.at(i).opcode) << " ";
        cout << setw(13)<<lines.at(i).operand << endl;
    }
}

/****************************************
 *Writes the listing file to a .lis file*
 ****************************************/
void sicxe_asm::write_file() {
//prints the listing file in proper format
    lis_filename = in_filename.substr(0,in_filename.size()-4);
    lis_filename = lis_filename+".lis";
    ofstream listing;
    listing.open(lis_filename.c_str(),ios::out);
    if(listing.is_open()){
        listing<<setw(25)<<"**"<<in_filename.substr(0,in_filename.size()-4)<<"**"<<endl;
        listing<<setw(7)<<"Line#"<<setw(12)<<"Address"<<setw(10)<<"Label"<<setw(14)<<"Opcode";
        listing<<setw(14)<<"Opcode"<<endl;
        listing<<setw(7)<<"====="<<setw(12)<<"======="<<setw(10)<<"====="<<setw(14)<<"======";
        listing<<setw(14)<<"======="<<endl;
        for(int i = 0; i < row_num; i++) {
          listing << setw(7)<<i+1<< " ";        
          listing << setw(11)<<format_7(lines.at(i).address)<<" ";
          listing << setw(9)<<format_7(lines.at(i).label) << " ";//Correct this since labels can be 8
          listing << setw(13)<<format_7(lines.at(i).opcode) << " ";
          listing << setw(13)<<lines.at(i).operand << endl;
        }
    }
    listing.close();
}

/*********************************
 *Formats a line to fill 7 spaces*
 *********************************/
string sicxe_asm::format_7(string x){
    stream<<setw(7)<<setfill(' ')<<x;
    string tmp = stream.str();
    stream.str("");
    return tmp;
}

/*************************************************
 *Verifies if given string is a hexidecimal value*
 *************************************************/
bool sicxe_asm::is_hex(string s){
  for(unsigned int i = 0; i < s.size(); i++){
      if(!isxdigit(s[i])) return false;  
  }
  return true;
}

/**************************************************
 *Verifies if the given string is a decimal number*
 **************************************************/
bool sicxe_asm::is_num(string s){
  for(unsigned int i = 0; i < s.size(); i++){
      if(!isdigit(s[i])) return false;  
  }
  return true;
}

/***********************************************
 *Converts a string from hexidecimal to integer*
 ***********************************************/
int sicxe_asm::hex_to_int(string s){
     int value;
     sscanf(s.c_str(),"%x",&value);
     return value;
}

/******************************************
 *Function to convert string to upper case*
 ******************************************/
string sicxe_asm::to_uppercase(string s){
    transform(s.begin(),s.end(),s.begin(),::toupper);
    return s;
}

/****************************************************
 *Counts the number of characters for a BYTE operand*
 ****************************************************/
int sicxe_asm::character_count(string s){
    int count=0;
    char c;
    if(s[s.size()-1]!= '\''){
        throw error_format("Invalid operand formatting::Operand: "+s);
    }
    /**Setting i = 1 to skip c or x and only count characters
     * between the quotes*/
    for(unsigned int i = 1;i<s.size();i++){
        c = s[i];
        if(i==1 && c != '\''){
            throw error_format("Invalid operand formatting: Operand: "+s);
        } 
        if(c=='\'')continue;
        count++;
    }
    return count;
}

/***************************************************************
 *Counts the byte operand characters based on c or x conditions*
 *if improperly formatted it will throw an error               *
 *otherwise return the number of bytes                         *
 ***************************************************************/
int sicxe_asm::count_byte_operand(string operand){
    int count = 0;
    string c = to_uppercase(operand.substr(0,1));
    if(c.compare("C")==0){
        count += character_count(operand);
        return count;
    }
    else if (c.compare("X")==0){
        int tmp = character_count(operand);
        /*If not an even number of characters or if not a hexidecimal value, throw*/
        if(tmp%2 !=0 && is_hex(operand.substr(2,operand.size()-3))){
            throw error_format("Invalid BYTE operand syntax: Operand: "+operand);
        }
        count += tmp/2;
        return count;
    }
    else
        throw error_format("Invalid BYTE operand syntax::Operand: "+operand);
}

/*****************************************
 *Counts the number of words to reserve  *
 *Error checking performed by dec_to_int * 
 *****************************************/
int sicxe_asm::count_resw_operand(string operand){
    int count = 0;
    count += dec_to_int(operand)*WORD_SIZE;
    return count;
}

/****************************************
 *Counts the number of Bytes to reserve *
 *Error checking performed by dec_to_int*
 ****************************************/
int sicxe_asm::count_resb_operand(string operand){
    int count = 0;
    count += dec_to_int(operand);
    return count;
}

/*******************************************
 *Takes an opcode and operand as parameters*
 *Processes any preprocessor directives    *
 *Returns value to be added to address     *
 *******************************************/
int sicxe_asm::process_directives(string label, string opcode, string operand){
    int count = 0;
    string tmp = to_uppercase(opcode);  
    
    //If start has already been declared, throw and error
    if(tmp.compare("START")==0){
       if(starting_address !=0){
            throw error_format("START has already been declared");
        }
        return count;
    }
    else if(tmp.compare("BYTE") ==0){
        count += count_byte_operand(operand);
        return count;        
    }
    else if(tmp.compare("RESW")==0){
        count += count_resw_operand(operand);
        return count;
    }
    else if(tmp.compare("WORD")==0){
        count += WORD_SIZE;
        return count;
    }
    else if(tmp.compare("RESB")==0){
        count += count_resb_operand(operand);
        return count;
    }
    else if(tmp.compare("EQU")==0){
        try{        
            if(is_num(operand)||operand[0]=='$'){
                symbol_table.insert_symbol(label, operand,"A");
                return count; 
            }        
            string rep_value;
            try{        
                rep_value = symbol_table.get_value(operand);   
                        
            }catch(symtab_exception syex){
                rep_value = operand;
            }       
            try{
                symbol_table.insert_symbol(label,rep_value,"R");
                return count;
            }catch(symtab_exception insex){
                throw error_format(insex.getMessage());
            }        
            
            throw error_format("Invalid operand for EQU command::Operand: "+operand);        
             
        }catch(symtab_exception symex){
            throw error_format(symex.getMessage());
        }catch(string other_ex){
            throw other_ex;
        }
        
        return count;
    }
    else if(tmp.compare("BASE")==0){
        base=operand;
        return count;
    }
    //Clear Base variable
    else if(tmp.compare("NOBASE")==0){
        base = "-1";
        return count;
    }
    //If start has not been declared, throw error
    else if(tmp.compare("END")==0){
        if(starting_address ==0){
            throw error_format("END called prior to start");
        }
        return count;
    }
    //Invalid preprocessor directive
    else{   
        return -1;        
    }
    
}


/**************************************************************************
 *Processes incomming starting address value                              *
 *If value is designated as hex it returns the hex equivalent value in int*
 *Otherwise returns the integer value of the variable                     *
 **************************************************************************/
int sicxe_asm::verify_start_location_value(string start_location_value){
    if(start_location_value[0]=='$'){
        start_location_value = start_location_value.substr(1,start_location_value.size());
        if(!is_hex(start_location_value)){
            throw error_format("Invalid starting address: "+start_location_value);
        }
        return hex_to_int(start_location_value); //Returns the hexidecimal value
    }
    else if(is_num(start_location_value)){
            return dec_to_int(start_location_value);        
    }
    else
        throw error_format("Invalid starting address: "+start_location_value); 
}

/***************************************************************************
 *Takes in a string message and returns the line number concat with message*
 *to form a throwable message                                              *
 ***************************************************************************/
string sicxe_asm::error_format(string message){
    ss_error<<"at line: "<<row_num+1<<"::"<<message;
    string tmp = ss_error.str();
    ss_error.str("");
    return tmp;
}

/******************************************
 *Converts string variables into integers *
 ******************************************/
int sicxe_asm::string_to_int(string s){
    istringstream instr(s);
    int n;
    instr >> n;
    return n;
 }
 
/**************************
 * Main Function          *
 **************************/
int main(int argc, char *argv[]){
    if(argc != 2){
        cout << "Error, you must supply the name of the file " <<
          "to process at the command line." << endl;
        exit(1);
    }
    new sicxe_asm(argv[1]);
    return 0;
}

