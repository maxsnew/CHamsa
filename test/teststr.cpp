#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;


int main() {

	char str1[]="hehe test ppmm";	
	str1[9]=0;

	string teststring(str1,13);
	string string1("test string1");
	string string2;

	string2=string1;
	string1[5]='G';

	cout <<"string1:" <<string1 <<endl;
	cout <<"string2:" <<string2 <<endl;

	cout << "str1: " << str1 <<endl;
	cout << "teststring:" << teststring <<endl;
	cout << "teststring.c_str():" << teststring.c_str()[10] <<endl;
	cout << "length=" <<teststring.length() <<endl;

	return 0;
}

