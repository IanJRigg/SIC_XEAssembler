/*  
    Ian Rigg, Richard Valles, Chris Andaya, Arron Mccarter
    masc1258
    file_parser.cc
    CS530, Spring 2014
*/

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

//constructor
file_parser::file_parser(string file_name) {
    in_file_name = file_name;
    v_counter=0;
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
        int operand_set=0; //Checks if operand has been tokenized
        int opcode_set=0; //Checks if opcode has been tokenized
        int had_single_quote=0; //Checks if a single quote has been processed
        stringstream ss,ss_error;
        contents.push_back(parsed_line()); //Creates each row
        for(unsigned int i=0; i< line.size();i++){
	    if(is_comment(line,i)){
               insert_to_vector("comment",contents,v_counter,line,i,line.size()-i,ss);
                break;
            }
            if(is_label(line,i)){
	    	if(!isalpha(line[i])){
		    throw_error(" 'Labels cannot start with a number'",ss_error);
		}
                while(!isspace(line[i])){
		    //Check to make sure contents are only alphanumeric or the null character
		    if(!isalnum(line[i])&&line[i]!=0){
		        throw_error(" 'Labels can only contain letters and numbers'",ss_error);
		    }
		    i++;
		}
		if(i>8){
		    insert_to_vector("label",contents,v_counter,line,0,8,ss);
		    continue;
		}
                insert_to_vector("label",contents,v_counter,line,0,i,ss);
                continue;
            }
            if(is_opcode(line,i,opcode_set)){
                int start = i;
                while(!isspace(line[i])){
                i++;
                }
                insert_to_vector("opcode",contents,v_counter,line,start,i-start,ss);
                opcode_set=1;
                continue;
            }
            if(is_operand(line,i,opcode_set,operand_set)){
                int start = i;
                while(!isspace(line[i])||(had_single_quote==1)){
                    if(line[i]=='\''){
                        had_single_quote++;
                    }
                i++;
                }
                insert_to_vector("operand",contents,v_counter,line,start,i-start,ss);
                operand_set=1;
                continue;
            }
            if(isspace(line[i])){
                continue;
            }
            if(has_too_many_tokens(line,i)){
	    	throw_error(" 'Too Many Tokens'",ss_error);
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
    //Prevents possible segmentation fault if wrong row selected
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

//Takes in a column location name, vector, vector row number, string to parse
//starting parse location, ending parse location, and stringstream variable
//Checks location and inputs into corresponding struct variable
//throws an error if location does not exist
//clears the stream
void file_parser::insert_to_vector(string location,vector<parsed_line>& contents,
			int i,string& line, int start,int end,stringstream& stream){
    stream<<line.substr(start,end)<<'\0';
    if(location.compare("comment")==0){ 
        contents[i].comment = stream.str();
    }
    else if(location.compare("label")==0){
        contents[i].label = stream.str();
    }
    else if(location.compare("opcode")==0){  
        contents[i].opcode = stream.str();
    }   
    else if(location.compare("operand")==0){ 
        contents[i].operand = stream.str();
    }
    else
        throw file_parse_exception("with insert_to_vector function, no such column");
    stream.str(""); 
 } 
 
 //Creates and throws a file_parse_exception
 void file_parser::throw_error(string error, stringstream& stream){
    stream<<"at line: "<<v_counter+1<<", in file "<<in_file_name<<error;
    throw file_parse_exception(stream.str());
 }
 
 //Checks if character is start of a comment
 int file_parser::is_comment(string line, int i){
    if(line[i] == '.'){
        return 1;
    }
    return 0;
 }
 
 //Checks if character should be an opcode
 int file_parser::is_opcode(string line, int i, int opcode_set){
    if((!isspace(line[i]))&&(!opcode_set)){
        return 1;
    }
    return 0;
 }
 
 //Checks if character should be an operand
 int file_parser::is_operand(string line,int i,int opcode_set,int operand_set){
    if((!isspace(line[i]))&&(opcode_set)&&(!operand_set)){
        return 1;
    }
    return 0;
 }
 
 //Checks if character is start of a label
 int file_parser::is_label(string line, int i){
    if((!isspace(line[i])) && (i==0)){
        return 1;
    }
    return 0;
 }
 
 //Checks if the line has too many tokens
 int file_parser::has_too_many_tokens(string line, int i){
    if((!is_comment(line,i))&&(!isspace(line[i]))){
        return 1;
    }
    return 0;
 }

