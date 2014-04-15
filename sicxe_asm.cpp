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
                second_pass();
	} catch (string error) {
		cout << "Error in sicxe_asm::first_pass(): " 
		<< "\n" << error << endl;
		exit(1);
	}
}

/************************************************************
 *Method: first_pass()                                      *
 ***********************************************************/
void sicxe_asm::first_pass(){
    file_parser parser(in_filename);
    try{
    parser.read_file();
    }catch(file_parse_exception ex){
        throw ex.getMessage();
    }    
    opcode = parser.get_token(row_num, 1); 
    string starting_hex_operand;    
    /*Checks opcode for start command                                     *
     * if not found, verifies that opcode is valid prior to initialization*
     * stores information and proceeds to next line                       */
    while(!string_compare(opcode,"START")){
         address = int_to_hex(int_location_counter);
         label = " ";
         opcode = " ";
         operand = " ";
         store_line();
         try{
         opcode = parser.get_token(++row_num, 1);
         }catch(file_parse_exception fex){
            throw error_format(fex.getMessage());
         }
    }    
    /* If opcode is start command then location is replaced with operand value
     * current address is stored on line
     */
    starting_hex_operand = parser.get_token(row_num, 2);
    label = parser.get_token(row_num,0);
    operand = parser.get_token(row_num,2);
    start_name = to_uppercase(label);
    address = int_to_hex(int_location_counter);
    store_line();
    /***************************************************** 
     *Checks if start is a hex declaration with $ symbol *
     * if not operand is taken for starting address value*
     *****************************************************/
    int_location_counter = verify_start_location_value(starting_hex_operand);
    starting_address = int_location_counter;
    row_num++;
    file_size = parser.size();
    while(row_num < file_size){         
        opcode = parser.get_token(row_num, 1);
        label = parser.get_token(row_num, 0);
        operand = parser.get_token(row_num,2);
        address = int_to_hex(int_location_counter);
        if(symbol_table.in_symtab(operand) /*&& string_compare(opcode,"EQU")*/){
            operand = symbol_table.get_value(operand);
        }         
        store_line();
        if(string_compare(opcode,"END")){
            if(!string_compare(opcode," ")){
                if(!string_compare(operand,start_name)){
                    throw error_format("Program end name does not match:(Operand: "+operand+") does not match start label (Label:"+start_name+")");
                }
            }              
            row_num++;
            break;
        }        
        if(!string_compare(label, " ") && !string_compare(opcode, "EQU")){
            try{
            symbol_table.insert_symbol(label,"$"+address,"R");
            }catch(symtab_exception symex){
                throw error_format(symex.getMessage());
            }
        }
        if(string_compare(opcode," ")){
           row_num++;
           continue;
        }
        int opcode_size =0;
        string errorflag="";
        try{
            opcode_size = opcode_table.get_instruction_size(opcode);
        } catch(opcode_error_exception ex){
            errorflag=ex.getMessage();            
        }
        if(errorflag.size()>1){
                opcode_size = process_directives();
                if(opcode_size==-1){
                    throw error_format(errorflag);
                }
        }
        int_location_counter+=opcode_size;
        row_num++;
    }
    prog_len = int_location_counter - starting_address;
    
}

/*****************************************************
 *Method: second_pass()                              *
 *****************************************************/
 void sicxe_asm::second_pass(){
    row_num =0;
    while(row_num < file_size-1){
        address = lines.at(row_num).address;
        opcode = lines.at(row_num).opcode;
        operand = lines.at(row_num).operand;
        cout<<check_addr_mode(operand)<<" ";
        row_num++;
    }
    cout<<endl;
    /*TODO: Get opcode and size, read and validate the operand field
     *operand for size 3/4 =
        -Alpha
        -#Alpha
        -@Alpha
        -#2  Constant value
        - Alpha, x - for LDX command
        -Blank  - Some functions take no operands
    -format 1 = Operand should be blank
    -format 2 = create register table to verify format2
    Verify size limits
    Calculate and set nixbpe flags
    */
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
void sicxe_asm::store_line(){
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
    cout<<setw(32)<<"**"<<in_filename.substr(0,in_filename.size()-4)<<"**"<<endl;
    cout<<format_8("Line#")<<format_15("Address")<<format_15("Label")<<format_15("Opcode")<<" ";
    cout<<format_15("Operand")<<format_15("Machine Code")<<endl;
    cout<<format_8("=====")<<format_15("=======")<<format_15("=====")<<format_15("======")<<" ";
    cout<<format_15("=======")<<format_15("============")<<endl;
    for(int i = 0; i < row_num; i++) {
        cout << setw(8)<<i+1;        
        cout << format_15(format_8(lines.at(i).address));
        cout << format_15(format_8(lines.at(i).label));
        cout << format_15(format_8(lines.at(i).opcode))<<" ";
        cout << format_15(lines.at(i).operand);
        cout<<format_15(lines.at(i).m_code)<<endl;
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
        listing<<setw(32)<<"**"<<in_filename.substr(0,in_filename.size()-4)<<"**"<<endl;
        listing<<format_8("Line#")<<format_15("Address")<<format_15("Label")<<format_15("Opcode")<<" ";
        listing<<format_15("Operand")<<format_15("Machine Code")<<endl;
        listing<<format_8("=====")<<format_15("=======")<<format_15("=====")<<format_15("======")<<" ";
        listing<<format_15("=======")<<format_15("============")<<endl;
        for(int i = 0; i < row_num; i++) {
            listing << setw(8)<<i+1;        
            listing << format_15(format_8(lines.at(i).address));
            listing << format_15(format_8(lines.at(i).label));
            listing << format_15(format_8(lines.at(i).opcode))<<" ";
            listing << format_15(lines.at(i).operand);
            listing << format_15(lines.at(i).m_code)<<endl;
        }
    }
    listing.close();
}

/*********************************
 *Formats a line to fill 8 spaces*
 *********************************/
string sicxe_asm::format_8(string x){
    stream<<setw(8)<<setfill(' ')<<x;
    string tmp = stream.str();
    stream.str("");
    return tmp;
}

/**********************************
 *Formats a line to fill 15 spaces*
 **********************************/
string sicxe_asm::format_15(string x){
    stream<<setw(15)<<setfill(' ')<<x;
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
int sicxe_asm::count_byte_operand(){
    int count = 0;
    string c = to_uppercase(operand.substr(0,1));
    if(string_compare(c,"C")){
        count += character_count(operand);
        return count;
    }
    else if (string_compare(c,"X")){
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
int sicxe_asm::count_resw_operand(){
    int count = 0;
    count += dec_to_int(operand)*WORD_SIZE;
    return count;
}

/****************************************
 *Counts the number of Bytes to reserve *
 *Error checking performed by dec_to_int*
 ****************************************/
int sicxe_asm::count_resb_operand(){
    int count = 0;
    count += dec_to_int(operand);
    return count;
}

/*******************************************
 *Takes an opcode and operand as parameters*
 *Processes any preprocessor directives    *
 *Returns value to be added to address     *
 *******************************************/
int sicxe_asm::process_directives(){
    int count = 0;
    string orig_operand = operand;
    //Checks for and replaces forward reference in operand
    if(symbol_table.in_symtab(operand)){
            operand = symbol_table.get_value(operand);
        }
    //If start has already been declared, throw and error
    if(string_compare(opcode,"START")){
       if(starting_address !=0){
            throw error_format("START has already been declared");
        }
        return count;
    }
    else if(string_compare(opcode,"BYTE")){
        count += count_byte_operand();
        return count;        
    }
    else if(string_compare(opcode,"RESW")){
        count += count_resw_operand();
        return count;
    }
    else if(string_compare(opcode,"WORD")){
        count += WORD_SIZE;
        return count;
    }
    else if(string_compare(opcode,"RESB")){
        count += count_resb_operand();
        return count;
    }
    else if(string_compare(opcode,"EQU")){
        process_equ();       
        return count;
    }
    else if(string_compare(opcode,"BASE") || string_compare(opcode,"NOBASE")){
        process_base();
        return count;
    }
    //If start has not been declared, throw error
    else if(string_compare(opcode,"END")){
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

/*********************************
 *Handles BASE directive commands*
 *********************************/
void sicxe_asm::process_base(){
    if(string_compare(opcode,"BASE")){
        base=operand;
    }
    else if(string_compare(opcode,"NOBASE")){
        base = "-1";
    }
}

/********************************
 *Handes EQU directive commands *
 ********************************/
void sicxe_asm::process_equ(){
        while(symbol_table.in_symtab(operand)){
            operand = symbol_table.get_value(operand);
        }
         try{        
            if(is_num(operand)||operand[0]=='$'){
                symbol_table.insert_symbol(label, operand,"A"); 
            }
            else{ 
                symbol_table.insert_symbol(label,operand,"R");
            } 
        }catch(symtab_exception symex){
            throw error_format(symex.getMessage());
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
 
/************************************************
 *Converts to uppercase and compares two strings*
 ************************************************/
bool sicxe_asm::string_compare(string first, string second){
    if(to_uppercase(first).compare(to_uppercase(second))==0){
        return true;
    }
    return false;
 }
 
 /***********************************************************
  *Checks first character of operand to see if #, @ or none *
  ***********************************************************/
int sicxe_asm::check_addr_mode(string operand){
    string first_char = operand.substr(0,1);
    if(string_compare(first_char, "#")){
        return 1;
    }
    else if(string_compare(first_char, "@")){
        return 2;
    }
    else if(string_compare(first_char, "$")){
        return 3;
    }
    else{
        return 0;
    }    
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

