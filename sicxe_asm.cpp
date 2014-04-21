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
    base_set = false;
    starting_address = 0;
    prog_len = 0;
    op_size = 0;
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
		cout << "Error in sicxe_asm::first_pass(): \n"; 
		cout << error << endl;
		exit(1);
	}
    try{
        second_pass();
    } catch(string pass_two_err){
        cout << "Error in sicxe_asm::second_pass(): \n";
        cout << pass_two_err <<endl; 
        exit(2);
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
         m_code= " ";
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
            symbol_table.insert_symbol(label,address,"R");
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
    a_reg = "0";
    b_reg = "3";
    x_reg = "1";
    s_reg = "4";
    t_reg = "5";
    l_reg = "2";
    sw_reg ="9";
    pc_reg ="8";
    int vector_size = lines.size();
    while(row_num < vector_size){
        n_bit = false;
        i_bit = false;
        x_bit = false;
        b_bit = false;
        p_bit = false;
        e_bit = false;        
        
        address = lines.at(row_num).address;
        opcode = lines.at(row_num).opcode;
        operand = lines.at(row_num).operand;
        int offset=0;
        try{
        /*Sets class variable op_size*/
        if(!string_compare(opcode, " ") && !is_process_directive(opcode)){
            op_size = opcode_table.get_instruction_size(opcode);
        }
        else{
            op_size =0;
        }
        }
        catch(opcode_error_exception op_err){
            throw error_format(op_err.getMessage());
        }
        if(string_compare(opcode,"word")){
            lines.at(row_num).m_code = "0"+int_to_hex(dec_to_int(operand));
        }        
        else if(string_compare(opcode,"byte")){
            //Process the hex byte directives
            if(string_compare(operand.substr(0,1),"x")){
                if(!is_hex(operand.substr(2,operand.size()-3))){
                    throw error_format("Invalid hexidecimal character");
                }
                lines.at(row_num).m_code = operand.substr(2,operand.size()-3);
            }
            //Process the character byte directives
            else{            
                string tmmp;
                string tempor = operand.substr(2,operand.size()-3);
                for(unsigned int i =0; i<tempor.size();i++){
                    int ascii = (int)tempor[i];
                    tmmp.append(int_to_hex(ascii).substr(3,2));
                }
                lines.at(row_num).m_code = tmmp;
            }
        }
        //Input a -- for visual clarity in the listing file
        else if(string_compare(opcode,"resw")||string_compare(opcode,"resb")){
            lines.at(row_num).m_code = "--";
        } 
        string orig_operand=operand;
        string parse1,parse2;
        parse_operand(orig_operand,parse1,parse2);
        int addr_type; 
        //Process the different format types      
        switch(op_size){
            case 1:
                process_format_one(opcode);
                break;
            case 2:
                if(!validate_registers(operand)){
                    throw error_format("Invalid Register in operand::"+operand);
                }
                break;
            case 3:
                offset = process_format_three_offset(operand);
                addr_type = check_addr_mode(orig_operand);
		if(isdigit(parse1[0])){
			if(addr_type == 0 && dec_to_int(parse1) > dec_to_int("4095"))
				throw error_format("Operand address out of range");
		}
                if(!validate_offset_size(int_to_hex(offset))){
                    //If base has not been set and the offset is out of range, throw an error
                    if(!base_set){
                        throw error_format("Operation invalid for PC relative and base not set");
                    }
                    b_bit = true;
                    if(addr_type>0){
                        orig_operand = orig_operand.substr(1,orig_operand.size());
                    }                    
                    process_forward_ref(base);
                    process_forward_ref(orig_operand);
                    offset =  hex_to_int(orig_operand)- hex_to_int(base);
                }
                else{
                    p_bit = true;
                }
                operand = validate_tf_operand(operand);
                //Handle some special cases for format 3                
                if((addr_type ==0 && !symbol_table.in_symtab(parse1))||
                    (is_hex(parse1)&&string_compare(parse2,"x"))||
                    ((parse1[0]=='@' || parse1[0]=='#')&&!symbol_table.in_symtab(parse1.substr(1,parse1.size())))){
                    b_bit = false;
                    p_bit = false;
                }
                break;
            case 4:                
                offset = process_format_four_offset(operand);
		if(isdigit(parse1[0])){
			if(addr_type == 0 && dec_to_int(parse1) > dec_to_int("1048575"))
				throw error_format("Operand address out of range");
		}
                if(!validate_offset_size(int_to_hex(offset))){
                    cout<<"Invalid size for format 4"<<endl;
                }
                operand = validate_tf_operand(operand);
                e_bit = true;                                              
                break;
            default:
                break;
        
        }
        //Combines flags with machine code for line
        unsigned int flags=0;
        if(n_bit){
            flags |= 0x0020000;
        } 
        if(i_bit){
            flags |= 0x0010000;
        }
        if(x_bit){
            flags |= 0x0008000;
        }
        if(b_bit){
            flags |= 0x0004000;
        }
        if(p_bit){
            flags|= 0x0002000;
        }
        if(e_bit){
            flags|= 0x0001000;
        } 
        unsigned int machine_code=0;
        //Only processes the modification if the type was 3 or 4
        if(op_size>2){
            machine_code = hex_to_int(opcode_table.get_machine_code(opcode));
        }
        machine_code<<=16;
        if(op_size==4){
            machine_code<<=8;
            flags<<=8;
        }               
        flags|=machine_code;        
        string tmp;
        if(flags!=0){            
                tmp = int_to_hex(offset);
                if(op_size==3){
                    if(tmp.size()>3)
                        tmp=tmp.substr(tmp.size()-3,3);
                    offset=hex_to_int(tmp);
                }   
                //If size 4 adjust to 32 bits                 
                if(op_size==4){
                    if(tmp.size()>5)
                        tmp = tmp.substr(tmp.size()-3,5);
                    offset=hex_to_int("0"+tmp);
                    offset|=0x00000000;
                    
                }
                flags|=offset;
                if(op_size==3){
                    stream<<setw(6)<<setfill('0')<<hex<<flags;
                }
                else if(op_size==4){
                    stream<<setw(8)<<setfill('0')<<hex<<flags;
                }    
                lines.at(row_num).m_code=to_uppercase(stream.str());
                stream.str("");
        } 
        row_num++;
    }    
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
    if(!is_num(s)&& s[0]!='-'){
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
        cout << format_15(lines.at(i).operand)<<"\t";
        cout << lines.at(i).m_code<<endl;
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
            listing << format_15(lines.at(i).operand)<<"\t";
            listing << lines.at(i).m_code<<endl;
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
    process_forward_ref(operand); 
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
        base_set = true;
    }
    else if(string_compare(opcode,"NOBASE")){
        base = "-1";
        base_set = false;
    }
}

/********************************
 *Handes EQU directive commands *
 ********************************/
void sicxe_asm::process_equ(){
        try{       
            if((is_num(operand)||operand[0]=='$')&& !symbol_table.in_symtab(operand)){
                operand = int_to_hex(dec_to_int(operand));
                symbol_table.insert_symbol(label, operand,"A"); 
            }
            else{                         
                //process_forward_ref(operand);
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
    else{
        return 0;
    }    
}

/***********************************************************
*Checks and confirms values in the operand field are valid.*
*Properly handles all cases including greyed out           *  
 **********************************************************/
string sicxe_asm::validate_tf_operand(string operand) {
    string op1;
    string op2=" ";
    int tmp = check_addr_mode(operand);
    parse_operand(operand, op1, op2);
    //Catches if operand was only a # or @ symbol
    if(string_compare(op1,"#")||string_compare(op1,"@")){
            throw error_format("Invalid Operand::"+op1);
    }
    if((tmp == 2 || tmp == 1)&& string_compare(op2," ")) {
        op1 = op1.substr(1,op1.size());
        process_forward_ref(op1);
        if((isdigit(op1[0])||string_compare(op1.substr(0,1),"-") ))
            return op1;
        else
            throw error_format("Invalid operand declared::\'"+op1+"\'");        
    }
    else{
        process_forward_ref(op1);
        if(!isdigit(op1[0]) && !string_compare(opcode,"rsub")){
            throw error_format("Invalid operand declared::\'"+op1+"\'");
        }
        if(string_compare(op2,"x")){                        
            x_bit = true;
        }
        else if(!string_compare(op2, " ")){
            throw error_format("Invalid operand declared::\'"+operand+"\'");
        }
        return op1;
    }    
}

/*************************************************
 *Verifies if the opcode is a processor directive*
 *************************************************/
bool sicxe_asm::is_process_directive(string opcode){    
    if(string_compare(opcode, "START") || string_compare(opcode,"BASE")|| 
        string_compare(opcode, "RESW") || string_compare(opcode, "BYTE")||
        string_compare(opcode, "WORD") || string_compare(opcode, "RESB")|| 
        string_compare(opcode, "EQU") || string_compare(opcode, "END")){
        return true;
    }
    else {
        return false;
        
    }
 }
 
/**************************************
 *validates the registers for format 2*
 **************************************/
bool sicxe_asm::validate_registers(string operand){
    string r1 = " ";
    string r2 = " ";
    string op_machine_code = opcode_table.get_machine_code(opcode);
    parse_operand(operand, r1, r2);
    if(r2.compare(" ")==0){
        if(!isdigit(r1[0])){
            string tmp = check_registers(r1);
            if(string_compare(tmp, "-1")){
                return false;
            }
            lines.at(row_num).m_code = op_machine_code+tmp+"0    "; 
        }
        else{
            stream<<hex<<string_to_int(r1);
            r1 = stream.str();
            stream.str("");
            r1= to_uppercase(r1);
            lines.at(row_num).m_code = op_machine_code+r1+"0    ";
        }
    }
    else{
        if(!isdigit(r2[0])){
            r1 = check_registers(r1);
            r2 = check_registers(r2);
            lines.at(row_num).m_code = op_machine_code+r1+r2+"    ";
        }
        else{
            r1 = check_registers(r1);
            int tmp = dec_to_int(r2);
            tmp = tmp-1;
            if(tmp<0){
                throw error_format("Invalid shift value in operand::"+operand);
            }
            stream<<op_machine_code<<r1<<tmp;
            r1 = stream.str();
            stream.str("");
            lines.at(row_num).m_code = r1;//op_machine_code+r1+"    ";
        }        
    }
    
    return true;
}

/******************************************************************
 *parses the operand into two reference variables                 *
 *op1,op2 are uninitialized string variable passed in by reference*
 ******************************************************************/
void sicxe_asm::parse_operand(string operand, string &op1, string &op2){
    size_t found = operand.find(",");
    op1 = operand.substr(0, found);
    if(found != std::string::npos){        
        op1 = operand.substr(0, found);
        op2 = operand.substr(found+1, operand.size());
    }
    
}
/******************************************************
 * Processes forward directives for referenced operand*
 ******************************************************/
void sicxe_asm::process_forward_ref(string &operand){
    while(symbol_table.in_symtab(operand)){
            operand = symbol_table.get_value(operand);
    }
}

/****************************************
 *validates if value given is a register*
 ****************************************/
string sicxe_asm::check_registers(string reg){
    if(string_compare(reg, "a")){
        return a_reg;
    }
    else if(string_compare(reg, "b")){
        return b_reg;
    }
    else if(string_compare(reg,"x")){
        return x_reg;
    }
    else if(string_compare(reg, "s")){
        return s_reg;
    }
    else if(string_compare(reg,"t")){
        return t_reg;
    }
    else if(string_compare(reg,"l")){
        return l_reg;
    }
    else if(string_compare(reg,"sw")){
        return sw_reg;
    }
    else if(string_compare(reg,"pc")){
        return pc_reg;
    }
    else
        return "-1";
}

/**********************************
 *processes the format one opcodes*
 **********************************/
void sicxe_asm::process_format_one(string opcode){
    if(operand.compare(" ")!=0){
        throw error_format("Invalid operand specified for format 1::"+operand);
    }
    lines.at(row_num).m_code = opcode_table.get_machine_code(opcode)+"      ";
}

/****************************************************
 *Verifies that the operands size is within range   *
 ****************************************************/
bool sicxe_asm::validate_offset_size(string offset) {
    if(string_compare(opcode,"rsub")){
        if(operand.compare(" ")==0){
            return true;
        }
        return false;        
    }
    //Verifies operand is in size 4 limitations for offset values 
    if(op_size == 4) {        
        if(!isdigit(operand[0])){            
            return (( dec_to_int("-524288") <= hex_to_int(offset))&&(hex_to_int(offset) <= dec_to_int("524287")));
        }
	else if(is_hex(offset)){
	    return ((hex_to_int(offset)==0) || (hex_to_int(offset) <= dec_to_int("1048575")));
        }        
	else{ 
	    return false;
        }
    }
    //Verifies operand is in size 3 limitations for offset values
    else if(op_size == 3) {
        if(!isdigit(operand[0])){
	    return ((dec_to_int("-2048") <= hex_to_int(offset))&&(hex_to_int(offset) <= dec_to_int("2047"))) ;
        }
        else if(is_hex(offset)){
	    return ((hex_to_int(offset)==0) || (hex_to_int(offset) <= dec_to_int("4095")));
        }
	else{
	    return false;
        }
    }
    return false;
}

/************************************************
 *Processes the offset for format 3 instructions*
 *returns an int value for the offset           *
 ************************************************/
int sicxe_asm::process_format_three_offset(string operand){
    string op1,op2;
    int destination=0;
    int source = hex_to_int(lines.at(row_num).address);
    int offset =0;
    int add_mode = check_addr_mode(operand);
    parse_operand(operand,op1,op2);
    bool op2_is_blank = string_compare(op2,"");
    string all_but_first = op1.substr(1,op1.size());
    process_forward_ref(op1);
    process_forward_ref(all_but_first);
    if(string_compare(opcode,"rsub")){
        if(!string_compare(operand," ")){
            throw error_format("Invalid operand for RSUB::"+operand);
        }
        n_bit=true;
        i_bit = true;
        return 0;
    }    
    if(string_compare(op2,"x")){
        if(is_hex(op1)){
            x_bit = true;
            n_bit = true;
            i_bit = true;
            destination= hex_to_int(op1);
            offset = destination - (source +3);
            return offset;
        }
        throw error_format("Operand is not of the form alpha,x or C,x::"+operand);
    }    
    if((add_mode!=0)&& op2_is_blank){
        if(add_mode ==2 && is_hex(all_but_first)){
            n_bit = true;
            destination = hex_to_int(all_but_first);            
        }
        else if(add_mode ==1 && (all_but_first[0]=='-' || is_num(operand.substr(1,operand.size())))){
            i_bit =true;
            destination = dec_to_int(all_but_first);
            return destination;
        }
        else if(add_mode ==1 && (is_hex(all_but_first))){
            destination = hex_to_int(all_but_first);
            i_bit = true;
        }
        else{
            throw error_format("Operand is invalid::"+operand);
        }
    }
    else if(is_hex(op1)){
        n_bit = true;
        i_bit = true;
        b_bit = false;
        p_bit = false;
        destination = hex_to_int(op1);
    }
    else{
        throw error_format("Invalid operand::"+operand);
    }
    offset = destination - (source +3);
    return offset;
}

/************************************************
 *Processes the offset for format 4 instructions*
 *returns an int value for the offset           *
 ************************************************/
int sicxe_asm::process_format_four_offset(string operand){
    string op1,op2;
    int destination=0;
    int offset =0;
    int add_mode = check_addr_mode(operand);
    parse_operand(operand,op1,op2);
    bool op2_is_blank = string_compare(op2,"");
    string all_but_first = op1.substr(1,op1.size());
    process_forward_ref(op1);
    process_forward_ref(all_but_first);
    if(string_compare(opcode,"rsub")){
        if(!string_compare(operand," ")){
            throw error_format("Invalid operand for RSUB::"+operand);
        }
        n_bit=true;
        i_bit = true;
        return 0;
    }    
    if(string_compare(op2,"x")){
        if(is_hex(op1)){
            x_bit = true;
            n_bit = true;
            i_bit = true;
            destination= hex_to_int(op1);
            offset = destination;
            return offset;
        }
        throw error_format("Operand is not of the form alpha,x or C,x::"+operand);
    }    
    if((add_mode!=0)&& op2_is_blank){
        if(add_mode ==2 && is_hex(all_but_first)){
            n_bit = true;
            return hex_to_int(all_but_first);            
        }
        else if(add_mode ==1 && (all_but_first[0]=='-' || is_num(operand.substr(1,operand.size())))){
            i_bit =true;
            destination = dec_to_int(all_but_first);
            return destination;
        }
        else if(add_mode ==1 && (is_hex(all_but_first))){            
            i_bit = true;
            return hex_to_int(all_but_first);
        }
        else{
            throw error_format("Operand is invalid::"+operand);
        }
    }
    else if(is_hex(op1)){
        n_bit = true;
        i_bit = true;
        b_bit = false;
        p_bit = false;
        //handles if number given 
        if(is_num(operand)){
            return dec_to_int(op1);
        }
        return hex_to_int(op1);
    }
    else{
        throw error_format("Invalid operand::"+operand);
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

