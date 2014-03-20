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
  file_parser parser(filename);
  parser.read_file();
  symtab symbol_table();
  opcodetab opcode_table();
  lines.reserve(500);
  row_num = 0;
}

sicxe_asm::~sicxe_asm(){}

void sicxe_asm::first_pass(){
    string opcode, location;
    int location_counter = 0;
    // Account for EQU statments
    while((opcode = parser.get_token(row_num, 1)) != "start"){
         if(opcode.compare(" ") != 0)
              throw_error("Command listed before program initialization");
         location = int_to_hex(location_counter);
         store_line(location, parser.get_token(row_num, 0), opcode, parser.get_token(row_num, 2));
         row_num++;
    }
    location = parser.get_token(row_num, 2); 
    if(location.find("$") != string::npos)
        location_counter = hex_to_int(location.substr(1, string::npos));
    else
        location_counter = hex_to_int(location);
    location = int_to_hex(location_counter);
    store_line(location_counter, parser.get_token(row_num, 0), parser.get_token(row_num, 1), parser.get_token(row_num, 2));
    
    row_num++;
    string label;
    // Account for labels at the beginning
    int opcode_size;
    unsigned int file_size = (unsigned int) parser.size();
    while(row_num < file_size){ // Check for "EOF"
        opcode = parser.get_token(row_num, 1);
        label = parser.get_token(row_num, 0);
        if(opcode.compare(" ") == 0){
           // if labels are kept here, insert the label into the symtab
           store_line(int_to_hex(location_counter), label, opcode, " ");
           row_num++;
           continue;
        }
        if(label.compare(" ") != 0){
            if(symbol_table.already_in_symtab(label))
                throw_error("Label already exists");
            symbol_table.insert(label, int_to_hex(location_counter));
        }
        try{
            opcode_size = opcode_table.get_instruction_size(opcode);
        } catch(opcode_error_exception ex){
            opcode_size = -1;
        }
        if(opcode_size > 0){
            location_counter += opcode_size;
        }
        else{
        }
        store_line(int_to_hex(location_counter), label, opcode, parser.get_token(row_num, 2));
        row_num++;
    }
    print_file();
}

 //Creates and throws a file_parse_exception
void sicxe_asm::throw_error(string error){
    ss_error<<"at line: "<<row_num + 1 << ", " << error;
    throw symtab_exception(ss_error.str());
}
string sicxe_asm::int_to_hex(int num){
    // Converts a decimal integer to a hexadecimal string.
    stringstream stream;
    stream << hex << num;
    return validate_address(stream.str());
}

void sicxe_asm::store_line(string address, string label, string opcode, string operand){
    prog_listing line;
    stringstream stream;
    stream <<row_num + 1;
    line.line_number = stream.str();
    line.address = address;
    line.label = label;
    line.opcode = opcode;
    line.operand = operand;
    lines.push_back(line);
}

void sicxe_asm::print_file() {
//prints the input file in proper format
    for(unsigned int i = 0; i <row_num; i++) {
        cout << lines.at(i).line_number << "\t";
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
     if(!is_hex(s))
         return 0;
     int value;
     sscanf(s.c_str(), "%X", &value);
     return value;
}

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
    catch(file_parse_exception ex){
        cout << ex.getMessage() << endl;
    }
    catch(symtab_exception ex){
        cout << ex.getMessage() << endl;
    }
    return 0;
}
