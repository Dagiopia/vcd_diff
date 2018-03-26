#ifndef VALCHANGE_H
#define VALCHANGE_H
#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

extern bool debugging;

class ValChange{

public:
	string nline, var_code, value;
	char *fname;
	char begin, change_type;
	int time, pos;
	bool end;
	char **keywords;
	ifstream vcdfile;

	ValChange(int enddefpos, char *Fname)
	{
		if(debugging) cout<<"\n in valchange constructor\n";
		//vcdfile.open(Fname);
		fname = Fname;
		pos = enddefpos;
		end = false;
		vcdfile.open(Fname);
		reset();
		if(debugging) cout<<"\nExiting valchange constructor\n";
	}

	~ValChange() { vcdfile.close(); }

	void getNextLine();
	void reset() {
		//vcdfile.close();
		//vcdfile.open(fname);
		vcdfile.seekg(pos);
		time = 0;
	}
	bool iskeyword(string);


};


#endif
