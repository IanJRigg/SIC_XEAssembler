#include <string>
#include <cstdlib>
#include <iostream>

#define apo 98

using namespace std;

int main(){
	string temp, other, another;
	temp = "this is a test to see' if this ' works";
	other = temp.c_str();
	int i = 0;
	while(other[i++] != '\''){
	}
	another.append("\'");
	int tempor = i + 1;
	while(other[tempor] != '\''){
		cout<<other[tempor++];
	}
	cout<<tempor<<endl;
	another.append(temp.substr(i, tempor - i + 1));
	cout<< another <<endl;
	return 0;
}
