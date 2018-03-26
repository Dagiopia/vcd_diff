#ifndef VCDDIFF_H
#define VCDDIFF_H

#include<vector>
#include<string>
#include"VCDparse.h"
#include"ValChange.h"
#include"g_func.h"

using namespace std;


extern bool debugging;

class diff{

public:
	VCDparse file1, file2;
	char *fname1, *fname2;
	bool sameNumScope, sameNumVar;
	string line, varcode1, varcode2;
	
	
	
	/*
		differences
					number of vars
					number of scopes
						for differences in above pick the one with most and then
						display those that are abscent in the other
					values in each var
	*/
	
	diff(char *file1name, char *file2name)
	{
		fname1 = file1name;
		fname2 = file2name;
		if(debugging) cout<<"\n*************in diff constructor\n";
		file1.setup(file1name);
		file2.setup(file2name);
		if(debugging) cout<<"\n*************done setting up objects\n";
		//cout<<"\n*************going to check scopes\n";
		//CheckScopes();
		//cout<<"\n*************done chaecking scopes\n";
		//CheckVars();
		//Compare(argb);//call this from main
		//cout<<"\ncccccccccccccccccccccccccccccccccccccccc\n";
	}


	~diff() { }

	//void Compare(argvals*); //used for checking specific scopes or references 
	void CheckVars();
	void CheckScopes();
	void CheckScopeNames();
	void CheckVarNames();
	void VarValueChange(string, string);
	void varvalchange(string);
	bool search(string, vector<string>);
	void CompareSig(map<int, string> *, map<int, string> *, string);
};

#endif