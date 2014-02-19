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
};

int main (int argc, char *argv[]) {
	ifstream infile;
	ofstream outfile;
	string line;
	int i, index, currIndex, v_counter = 0;
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
		contents.push_back(parsed_line());
		for(unsigned int i=0; i< line.size();i++){
			if(line[i] == '.'){
				contents[v_counter].comment = line.substr(i,line.size()-i+1);
				break;
			}
		}
		v_counter++;
	}
	
	infile.close();

	for (unsigned int i=0; i<v_counter;i++) {
		
		cout << contents[i].comment << endl;
	}

	return 0;
}
