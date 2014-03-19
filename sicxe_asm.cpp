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
  source_file_name = filename;
  symtab symbol_table();
  opcodetab opcode_table();
  lines.reserve(500);
  size = 0;
}

sicxe_asm::~sicxe_asm(){}

void sicxe_asm::first_pass(){
  	file_parser parser(source_file_name);
  	parser.read_file();
    string opcode;
    while((opcode = parser.get_token(size, 1)) != "start"){
        if(opcode.compare(" ") != 0)
            throw_error("Command listed before program initialization");
        write_to_lines("00000", "", "", "");
        size++;
    }
    string location = parser.get_token(size, 2);
    string location_counter, start_location;
    if(location.find("$") != string::npos)
        location_counter = location.substr(1, string::npos);
    else
        location_counter = itohs(location);
    write_to_lines("00000", parser.get_token(size, 0), parser.get_token(size, 1), location);
    
    size++;
    location_counter = validate_address(location_counter);
    start_location = location_counter;
    string label;
    // Account for labels at the beginning
    int opcode_size;
    unsigned int file_size = (unsigned int) parser.size();
    while(size < file_size){ // Check for "EOF"
        opcode = parser.get_token(size, 1);
        label = parser.get_token(size, 0);
        if(opcode.compare(" ") == 0){
            // if labels are kept here, insert the label into the symtab
            write_to_lines(location_counter, label, " ", " ");
            size++;
            continue;
        }
        if(label.compare(" ") != 0){
            if(symbol_table.already_in_symtab(label))
                throw_error("Label already exists");
            symbol_table.insert(label, location_counter);
        }
        try{
            opcode_size = opcode_table.opcode_exists(opcode); // private method
        } catch(opcode_error_exception ex){
            opcode_size = -1;
        }
        if(opcode_found > 0){
            opcode_size = opcode_table.get_instruction_size(opcode);
            location_counter = increment_location_counter(location_counter, opcode_size);
        }
        //else if(){ // Handle directives here
        //}
        /*else{
            throw symtab_exception("Uknown command on line: ");
        }*/
        write_to_lines(location_counter, label, opcode, parser.get_token(size, 2));
        size++;
    }
    print_file();
}

 //Creates and throws a file_parse_exception
void sicxe_asm::throw_error(string error){
    ss_error<<"at line: "<< size + 1 << ", " << error;
    throw symtab_exception(ss_error.str());
}
string sicxe_asm::itohs(string dec_string){
    // Converts a decimal integer to a hexadecimal string.
    stringstream stream;
    stream << dec_string;
    string temp;
    stream >> hex >> temp;
    return temp;
}

void sicxe_asm::write_to_lines(string address, string label, string opcode, string operand){
    prog_listing line;
    stringstream stream;
    stream << size + 1;
    line.line_number = stream.str();
    line.address = address;
    line.label = label;
    line.opcode = opcode;
    line.operand = operand;
    lines.push_back(line);
}

void sicxe_asm::print_file() {
//prints the input file in proper format
    for(unsigned int i = 0; i < size; i++) {
        cout << lines.at(i).line_number << "\t";
        cout << lines.at(i).address << "\t";
        cout << lines.at(i).label << "\t";
        cout << lines.at(i).opcode << "\t";
        cout << lines.at(i).operand << endl;
    }
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
string sicxe_asm::increment_location_counter(string location_counter, unsigned int opcode_size){
    unsigned int loc_ctr;
    stringstream stream;
    stream << hex << location_counter;
    stream >> loc_ctr;
    loc_ctr += size;
    stream.str("");
    stream << hex << loc_ctr;
    cout << loc_ctr << endl;
    return stream.str();
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
