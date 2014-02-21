#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include "file_parse_exception.h"

using namespace std;

struct parsed_line{
    string label;
    string opcode;
    string operand;
    string comment;
    parsed_line(): label(""),
                   opcode(""),
                   operand(""),
                   comment(""){}
};

int main (int argc, char *argv[]) {
    ifstream infile;
    ofstream outfile;
    string line;
    int i, index,currIndex, v_counter = 0;
    vector<parsed_line> contents;
    vector<parsed_line>::iterator v_iter;

    if (argc != 2) {
        cout << "Error in argc!" << endl;
        return 1;
    }

    infile.open(argv[1], ios::in);
    if (!infile) {
        cout << "Error in infile!" << endl;
        return 1;
        }

    while (!infile.eof()) {
        getline(infile, line);
        int operand_set=0;
        int opcode_set=0;
        int start =0;
        contents.push_back(parsed_line());
        for(unsigned int i=0; i< line.size();i++){
            if(line[i] == '.'){
                contents[v_counter].comment = line.substr(i,line.size()-i+1);
                break;
            }
            if((!isspace(line[i])) && (i==0)){
                while(!isspace(line[i])){
                    i++;
                    }
                contents[v_counter].label= line.substr(0,i);
                continue;
            }
            if((!isspace(line[i]))&&(!opcode_set)){
                start = i;
                while(!isspace(line[i])){
                i++;
                }
                contents[v_counter].opcode=line.substr(start,i-start);
                opcode_set =1;
                continue;
            }
            if((!isspace(line[i]))&&(opcode_set)&&(!operand_set)){
                start = i;
                while(!isspace(line[i])){
                i++;
                }
                contents[v_counter].operand=line.substr(start,i-start);
                operand_set =1;
                continue;
            }
            if(isspace(line[i])){
                continue;
            }
            //Need to code this to throw exception
            if((line[i]!='.')&&(!isspace(line[i]))){
                cout<<"Error with "<<line.substr(i,10)<<endl;
                break;
            }


        }
        v_counter++;
    }

    infile.close();

    for (unsigned int i=0; i<v_counter;i++) {
        cout << contents[i].label<<"\t";
        cout << contents[i].opcode<<"\t";
        cout << contents[i].operand<<"\t";
        cout << contents[i].comment<< endl;
    }

    return 0;
}