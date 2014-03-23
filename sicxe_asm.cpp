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
    base = 0;
    starting_address = 0;
    prog_len = 0;
}

sicxe_asm::~sicxe_asm(){}

void sicxe_asm::first_pass(){
    file_parser parser(in_filename);
    try{
    parser.read_file();
    }catch(file_parse_exception ex){
        throw ex.getMessage();
    }
    
    string opcode = parser.get_token(row_num, 1); 
    string hex_location_counter;
    
    /* Checks opcode for start command
     * if not found, verifies that opcode is valid prior to initialization
     * stores information and proceeds to next line*/
    while(to_uppercase(opcode).compare("START") != 0){
         if(opcode.compare(" ")!=0){
                string tmp_error = "Invalid command prior to program initialization: Opcode:"+opcode;                
                if(process_directives(parser.get_token(row_num,0),opcode,parser.get_token(row_num,2)) == 0){
                    store_line(int_to_hex(int_location_counter),parser.get_token(row_num,0),opcode, parser.get_token(row_num,2));
                    opcode=parser.get_token(++row_num,1);
                    tmp_error = "";
                    continue;
                }
                throw error_format(tmp_error);
         }     
         hex_location_counter = int_to_hex(int_location_counter);
         store_line(hex_location_counter, parser.get_token(row_num, 0), opcode, parser.get_token(row_num, 2));
         opcode = parser.get_token(++row_num, 1);
    }
    
    /* If opcode is start command then location is replaced with operand value
     * current address is stored on line
     */
    hex_location_counter = parser.get_token(row_num, 2); 
    store_line(int_to_hex(int_location_counter), parser.get_token(row_num, 0), parser.get_token(row_num, 1), parser.get_token(row_num, 2));
    /* Checks if start is a hex declaration with $ symbol
     * if not operand is taken for starting address value
     */
    int_location_counter = verify_start_location_value(hex_location_counter);
    starting_address = int_location_counter;
    hex_location_counter = int_to_hex(int_location_counter);
    row_num++;
    string label;
    // Account for labels at the beginning

    int file_size = parser.size();
    while(row_num < file_size){ // Check for "EOF"
        
        opcode = parser.get_token(row_num, 1);
        label = parser.get_token(row_num, 0);
        string operand = parser.get_token(row_num,2);
        store_line(int_to_hex(int_location_counter), label, opcode, operand);
        if(opcode.compare(" ") == 0){
           row_num++;
           continue;
        }
        if(to_uppercase(opcode).compare("END")==0){
            string filecheck = in_filename.substr(0,in_filename.size()-4);
            if(to_uppercase(operand).compare(to_uppercase(filecheck))!=0){
                throw error_format("Program name does not match: (Operand: "+operand+") does not match (Filename:"+filecheck+")");
            }
            row_num++;
            break;
        }
        
        if(label.compare(" ") != 0 && to_uppercase(opcode).compare("EQU")!=0){
            //Verify case for this call TODO
            symbol_table.insert_symbol(label, int_to_hex(int_location_counter),"R");
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

// Converts a decimal integer to a hexadecimal string.
string sicxe_asm::int_to_hex(int num){
    stream<<setw(5)<<setfill('0')<<hex<<num;
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
        listing << setw(9)<<format_7(lines.at(i).label) << " ";
        listing << setw(13)<<format_7(lines.at(i).opcode) << " ";
        listing << setw(13)<<lines.at(i).operand << endl;
    }
    }
    listing.close();
}

string sicxe_asm::format_7(string x){
    stream<<setw(7)<<setfill(' ')<<x;
    string tmp = stream.str();
    stream.str("");
    return tmp;
}

bool sicxe_asm::is_hex(string s){
  for(unsigned int i = 0; i < s.size(); i++){
      if(!isxdigit(s[i])) return false;  
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
            throw error_format("Invalid operand formatting: Operand: "+s);
        } 
        if(c=='\'')continue;
        count++;
    }
    return count;
}

//TODO verify error checking!!!!!
int sicxe_asm::count_byte_operand(string operand){
    int count = 0;
    string c = to_uppercase(operand.substr(0,1));
    if(c.compare("C")==0)
        count += character_count(operand);
    if (c.compare("X")==0){
        int tmp = character_count(operand);
        if(tmp%2 !=0 || !is_hex(operand.substr(2,operand.size()-3))){
            throw error_format("Invalid BYTE operand syntax: Operand: "+operand);
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
int sicxe_asm::process_directives(string label, string opcode, string operand){
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
        try{
        if(is_hex(operand)){
            symbol_table.insert_symbol(label, operand,"A"); 
        }
        else{
            throw error_format("Invalid operand for EQU command::Operand: "+operand);        
        }  
        }catch(symtab_exception symex){
            throw error_format(symex.getMessage());
        }catch(string other_ex){
            throw other_ex;
        }
        
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
    //GET RID OF THIS WHEN REFACTORED!!!!
    else if(tmp.compare("END")==0){
        return count;
    }
    else{   
        return -1;        
    }
    
}

int sicxe_asm::verify_start_location_value(string hex_location_counter){
    int count=0;
    if(hex_location_counter[0]=='$'){
        hex_location_counter = hex_location_counter.substr(1,hex_location_counter.size());
    }
    
    if(!is_hex(hex_location_counter)){
        throw error_format("Invalid starting address: "+hex_location_counter);
    }
    else{
        count = hex_to_int(hex_location_counter);
    }
    return count;
}

string sicxe_asm::error_format(string message){
    ss_error<<"at line: "<<row_num+1<<"::"<<message;
    string tmp = ss_error.str();
    ss_error.str("");
    return tmp;
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

