#include<iostream>
#include "diff.h"

using namespace std;
bool debugging;
int main(int argc, char **argv)
{
	debugging = true;
	diff VCD(argv[1], argv[2]);
	cout<<"Hello world!\n";
return 0;
}
