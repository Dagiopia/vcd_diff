#include<iostream>
#include<Windows.h>
#include<fstream>
#include"VCDparse.h"
#include"diff.h"

using namespace std;

void main(int argc, char **argv)
{
	diff::argvals argb;

	diff VCDdiff(argv[1], argv[2], &argb);

	VCDdiff.varvalchange(argv[3]);

}