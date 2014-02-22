/*  file_parser.cc
    CS530, Spring 2014
*/

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
	in_file_name = file_name;
	set_default_values();
}

//destructor
file_parser::~file_parser() {
}

void file_parser::read_file() {
//parsing code and storage into vector<struct>
	infile.open(in_file_name.c_str(), std::ifstream::in);
	if (!infile) {
		throw file_parse_exception("source code file does not exist.");
	}
	
	while (!infile.eof()) {
        getline(infile, line);
	//Do we even need to have setters for these since they only exist in the
	//while loop?
	set_operand_set(0);
        set_opcode_set(0);
        int had_single_quote=0;
	stringstream ss_comment,ss_opcode,ss_operand,ss_label,ss_error;
        contents.push_back(parsed_line());
        for(unsigned int i=0; i< line.size();i++){
	    if(line[i] == '.'){
                //Want to add method to handle each case cleanly
                ss_comment<<line.substr(i,line.size()-i)<<'\0';
                contents[v_counter].comment = ss_comment.str();
                ss_comment.str("");
                break;
            }
            if((!isspace(line[i])) && (i==0)){
	    	if(!isalpha(line[i])){
		    ss_error<<"at line: "<<v_counter+1<<", in file "<<in_file_name<<" 'Labels cannot start with a number'";
                    throw file_parse_exception(ss_error.str());
		}
                while(!isspace(line[i])){
                    i++;
                    }
                ss_label<<line.substr(0,i)<<'\0';
                contents[v_counter].label= ss_label.str();
                ss_label.str("");
                continue;
            }
            if((!isspace(line[i]))&&(!opcode_set)){
                int start = i;
                while(!isspace(line[i])){
                i++;
                }
                ss_opcode<<line.substr(start,i-start)<<'\0';
                contents[v_counter].opcode=ss_opcode.str();
                set_opcode_set(1);
                ss_opcode.str("");
                continue;
            }
            if((!isspace(line[i]))&&(opcode_set)&&(!operand_set)){
                int start = i;
                while(!isspace(line[i])||(had_single_quote==1)){
                    if(line[i]=='\''){
                        had_single_quote++;
                    }
                i++;
                }
                ss_operand<<line.substr(start,i-start)<<'\0';
                contents[v_counter].operand=ss_operand.str();
                set_operand_set(1);
                ss_operand.str("");
                continue;
            }
            if(isspace(line[i])){
                continue;
            }
            //Need to code this to throw exception
            if((line[i]!='.')&&(!isspace(line[i]))){
	    	ss_error<<"at line: "<<v_counter+1<<", in file "<<in_file_name<<" 'Too Many Tokens'";
                throw file_parse_exception(ss_error.str());
            }
        }
        v_counter++;
    }
    infile.close(); 
}

string file_parser::get_token(unsigned int r, unsigned int c) {
//gets the token located at
	unsigned int column = c;
	unsigned int row = r;
	string token;
        //Wrote this to prevent possible segmentation fault if wrong row selected
        if(contents.size()<row){
            stringstream ss_error;
            ss_error<<"no such row: "<<row<<" in file "<<in_file_name;
            throw file_parse_exception( ss_error.str());
        }

	switch (column) {
		case 0:
			return contents[row].label;
		case 1:
			return contents[row].opcode;
		case 2:
			return contents[row].operand;
		case 3:
			return contents[row].comment;
		default:
			return "";
	}
}

void file_parser::print_file() {
//prints the input file in proper format
	for (int i = 0; i < v_counter; i++) {
		cout << contents[i].label << "\t";
		cout << contents[i].opcode << "\t";
		cout << contents[i].operand << "\t";
		cout << contents[i].comment << endl;
	}
}

int file_parser::size() {
//returns the number of lines in the source code file
    return contents.size();
}

void file_parser::set_operand_set(int a) {
    operand_set = a;
}

void file_parser::set_opcode_set(int a) {
    opcode_set = a;
}

void file_parser::set_default_values(){
    operand_set=0;
    opcode_set=0;
}

