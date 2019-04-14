#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include"diff.h"


using namespace std;

void diff::CheckScopes()
{
	if(file1.num_scope == file2.num_scope){
		sameNumScope = true;
		cout<<"Num scopes is the same";
	}
	else{
		sameNumScope = false;
		cout<<"Num of scopes is different "<<file1.num_scope<<" "<<file2.num_scope;
	}
//	CheckScopeNames(); //call moved to Compare(string, string)
}
 
void diff::CheckVars()
{
	if(file1.num_var == file2.num_var){
		sameNumVar = true;
		cout<<"\nNum Variables is the same";
	}
	else{
		sameNumVar = false;
		cout<<"\nNum of variables is different "<<file1.num_var<<" "<<file2.num_var;
	}
	
//	CheckVarNames(); //call moved to Compare(string, string)
}

void diff::CheckScopeNames()
{
	CheckScopes();
	/*
		compare the names and scope types of the scopes in both files
			report 
				-which scopes are abscent in which file
		does_exit function to see if a certain scope exists in the other file
	*/
	bool found = false;
	vector<string> NOTin2, //holds those in file1 but not in file2 
				   NOTin1; //holds those in file2 but not in file1 
	for(int i = 0; i < file1.num_scope; i++){//loop through scopes of first file comparing
		//each to those in the second file and if it's not found, put it in NOTin2
		found = false;
		for(int j = 0; j < file2.num_scope; j++){
			if(!file1.scopeID[i].compare(file2.scopeID[j])){
				//cout<<"found "<<file1.scopeID[i]<<endl;
				found = true;
				break;
			}
		}
		if(!found && !Does_exist(NOTin2, file1.scopeID[i]))
				NOTin2.push_back(file1.scopeID[i]);

	}

	for(int i = 0; i < file2.num_scope; i++){//loop through scopes of second file comparing
		//each to those in the first file and if it's not found, put it in NOTin1
		found = false;
		for(int j = 0; j < file1.num_scope; j++){
			if(!file2.scopeID[i].compare(file1.scopeID[j])){
				found = true;
				break;
			}
		}
		if(!found && !Does_exist(NOTin1, file2.scopeID[i]))
				NOTin1.push_back(file2.scopeID[i]);
	}

	//display diff in scope
	cout<<"\nNumber of Scopes in file1 but not in file2 = "<<NOTin2.size();
	if(NOTin2.size() > 0){
		cout<<"\nThose in 1 but not 2 ->\n";
		for(int i = 0; i < NOTin2.size(); i++){
			cout<<"\t"<<NOTin2[i]<<endl;
		}
	}

	cout<<"\nNumber of Scopes in file2 but not in file1 = "<<NOTin1.size();
	if(NOTin1.size() > 0){
		cout<<"\nThose in 2 but not 1 ->\n";
		for(int i = 0; i < NOTin1.size(); i++){
			cout<<"\t"<<NOTin1[i]<<endl;
		}
	}


/*	for(int i = 0; i < file1.num_scope; i++){
		cout<<file1.scopeID[i]<<"\t"<<file2.scopeID[i]<<endl;
	}*/
}



void diff::CheckVarNames()
{
	CheckVars();
	//esti try doing the ref name comparision using ref_man
	//u can use the iterator with find.. u can do it in 5 lines minamin

	bool found = false;
	vector<string> NOTin2, //holds those in file1 but not in file2 
				   NOTin1; //holds those in file2 but not in file1 
	for(int i = 0; i < file1.num_var; i++){//loop through vars of first file comparing
		//each to those in the second file and if it's not found, put it in NOTin2
		found = false;
		for(int j = 0; j < file2.num_var; j++){
			if(!file1.variable_man[i].reference.compare(file2.variable_man[j].reference)){
				found = true;
				break;
			}
		}
		if(!found && !Does_exist(NOTin2, file1.variable_man[i].reference))
			NOTin2.push_back(file1.variable_man[i].reference);

	} 

	for(int i = 0; i < file2.num_var; i++){//loop through vars of second file comparing
		//each to those in the first file and if it's not found, put it in NOTin1
		found = false; 
		for(int j = 0; j < file1.num_var; j++){
			if(!file2.variable_man[i].reference.compare(file1.variable_man[j].reference)){
				found = true;
				break;
			}
		}
		if(!found && !Does_exist(NOTin1, file2.variable_man[i].reference))
			NOTin1.push_back(file2.variable_man[i].reference);

	}

	//display diff in scope
	cout<<"\nNumber of Variables in file1 but not in file2 = "<<NOTin2.size();
	if(NOTin2.size() > 0){
		cout<<"\nThose in 1 but not 2 ->\n";
		for(int i = 0; i < NOTin2.size(); i++){
			cout<<"\t"<<NOTin2[i]<<endl;
		}
	}

	cout<<"\nNumber of Variables in file2 but not in file1 = "<<NOTin1.size();
	if(NOTin1.size() > 0){
		cout<<"\nThose in 2 but not 1 ->\n";
		for(int i = 0; i < NOTin1.size(); i++){
			cout<<"\t"<<NOTin1[i]<<endl;
		}
	}
}


//modify this function to take arguments... string ref_name, string reff_address
//and modify the rest so as the vars are obtained from the map<string, map<string, vector<string>>>
void diff::VarValueChange(string ref_id, string ref_address)
{
	ValChange FILE1(file1.enddef_pos, fname1), 
			  FILE2(file2.enddef_pos, fname2);
	bool varfound = false, var1found = false, var2found = false, end = false;
	char status;
	//i'm gonna use subscripting to find the data form the map...
	//so make sure u send the right data to this function
	//data that isn't given by the user or anything else... it should be the data directly obtained from
	//VCDparse objects
	string varof1(file1.address_var[ref_id][ref_address]), 
				   varof2(file2.address_var[ref_id][ref_address]);
	
	map<int, string> sig_f1, sig_f2;
		
	if(debugging) cout<<endl<<varof1<<"\t\t"<<varof2;
	
	while(!FILE1.end && !FILE2.end){//if it keeps finding the vars untill the end
		
		FILE1.getNextLine();
		FILE2.getNextLine();
		if(!FILE1.var_code.compare(varof1)){
			sig_f1[FILE1.time*file1.time_scale] = FILE1.value;
		}
		if(!FILE2.var_code.compare(varof2)){
			sig_f2[FILE2.time*file2.time_scale] = FILE2.value;
		}	
	}
	if(debugging) cout<<"\n\nfinished the while file !end loop... going into CompareSig()"<<sig_f1.size()<<"  "<<sig_f2.size();
	CompareSig(&sig_f1, &sig_f2, ref_id);

}

//this function is to be removed
//given a var_code this function prints the value changes of that variable at each time
void diff::varvalchange(string varL)
{
	cout<<"\n=============================================================";
	ValChange FILE1(file1.enddef_pos, fname1);
	FILE1.end = false;
	int prev = 0;
	while(!FILE1.end){
		FILE1.getNextLine();
		//cout<<FILE1.var_code <<" = "<<FILE1.value<<"        "<<FILE1.time<<endl;
		prev = FILE1.time;
		if(!varL.compare(FILE1.var_code))
			cout<<endl<<FILE1.var_code<<"    "<<FILE1.value<<"           "<<FILE1.time*file1.time_scale<<" ps";
	}
	cout<<"\n=============================================================\n";
}

void diff::CompareSig(map<int, string> *sig1, map<int, string> *sig2, string ref_na)
{
	map<int, string>::const_iterator sig1_it = sig1->begin();
	map<int, string>::const_iterator sig2_it = sig2->begin();
	if(debugging){ 
		cout<<"\n\ngoing to print signal map values";
		cout<<endl<<endl<<endl;
		while(sig1_it != sig1->end() && sig2_it != sig2->end()){
			cout<<sig1_it->second<<"    "<<sig1_it->first<<"                 "<<sig2_it->second<<"   "<<sig2_it->first<<endl;
			sig1_it++; sig2_it++;
		}
	}
	if((*sig1) == (*sig2)){
		printf("\nThe Signals are the same");
	}
	else{
		sig1_it = sig1->begin();
		sig2_it = sig2->begin();
		while(sig1_it != sig1->end() && sig2_it != sig2->end()){
			if(sig1_it->first != sig2_it->first ||
				sig1_it->second.compare(sig2_it->second) != 0)
				printf("\n%s \t %s = %s  at time %d %s\n%s \t %s = %s  at time %d %s",
				fname1, ref_na.c_str(), sig1_it->second.c_str(), sig1_it->first, file1.time_unit.c_str(),
				fname2, ref_na.c_str(), sig2_it->second.c_str(), sig2_it->first, file2.time_unit.c_str());
			sig1_it++; sig2_it++;
		}
	}
	
}


bool diff::search(string VAR, vector<string> VARS)
{
	for(int i = 0; i < VARS.size(); i++)
		if(!VAR.compare(VARS[i]))
			return 1;

	return 0;
}

