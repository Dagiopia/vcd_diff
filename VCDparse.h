#ifndef VCD_PARSE_H
#define VCD_PARSE_H
#include<iostream>
#include<fstream>
#include<string.h>
#include<algorithm>
#include<vector>
#include<map>


using namespace std;

extern bool debugging;

class VCDparse {

private:
	struct varman{
		std::string type;
		int size;
		string id_code;
		string refnobit;
		string reference;
		string parent_name;
		string address;
	};

public:
	char *scope, *end, *upscope, *var, *enddef, *comment, *date, *timescale,
		 *version, *dumpvars, *dumpoff, *dumpon, *dumpall;
	//char *file1name, *file2name;

	int num_scope, num_upscope, num_var, time_scale;
	string time_unit;
	int enddef_pos, st_time, end_time;
	char *buffer;
	varman *variable_man;

	vector<string> definitions;

	//map<string, vector<string>> ref_man; //ref name as key and vector of vars as value
	//map<string, vector<string>> parent_ref_var; //scopeID and ref as key and vector of vars as value
	//map<string, string> var_ref_man; //var code as key and ref name as value
	map<string, map<string, string> > address_var;//reference , address, vector of vars

	map<string, vector<string> > scope_address;//scope name as key and vector of addresses as value

	//parallel values
	vector<string> scopeID;
	vector<int> scopedepth;
	int depth, index, var_count;
	string scope_type, scope_name;


	VCDparse(char *filename)
	{
		if(debugging) cout<<"\nIN VCDparse constructor\nGoing into setup()\n";
		setup(filename);
		//depth = 0;	//var_count = 0;   //	index = 0;
		if(debugging) cout<<"\nback in VCDparse constructor\n\n";
	}

	VCDparse() {}

	~VCDparse()
	{
		if(debugging) cout<<"\nin VCDparse destructor";
		delete [] buffer;
		delete [] variable_man;
		if(debugging) cout<<"\nexiting destructor\n\n";
	}

	size_t GetFSize(FILE *);//returns the size of a file in bytes
	void setup(char *);

private:

	int getEndDefPos(char *);//get the position of $enddefintions
	int Definitions_section(char *); //returns size of the buffer that contains the file up to $enddefifnitions
//  ^this function opens the file from the argument and then openes that file and copies
//  its contents upto $enddefinitions into the buffer and returns the size in bytes(# of chars)

	void get_startend_time(ifstream *);
	void varManager(char *);
	void refManager();
	bool ischild(string, string);
	//bool does_exist(vector<string>, string);

};

#endif
