#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

void main (int argc, char *argv[]) {
	ifstream infile;
	ofstream outfile;
	vector<string> contents;
	string line;
	
	if (argc != 2) {
		cout << "Error in argc!" << endl;
		return;
	}
	
	infile.open(argv[1], ios::in);
	if (!infile) {
		cout << "Error in infile!" << endl;
		return;
	}
	
	while (!infile.eof()) {
		getline(infile, line);
		contents.pushback(line);
	}
	
	infile.close();
	
	for (int i = 0; i < contents.size(); i++) {
		cout << contents[i] << endl;
	}
	
	outfile.open("output.txt", ios::out);
	if (!outfile) {
		cout << "Error in outfile!" << endl;
		return;
	}
	
	for (i = 0; i < contents.size(); i++) {
		cout << contents[i] << endl;
	}
	outfile.close();
}