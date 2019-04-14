/*
	Dagim Sisay 
		July, 2015
*/
#include<iostream>
#include<fstream>

#ifdef _WIN32_
	#include<Windows.h>
	#include<conio.h>
	#include<string>
#else
	#include<time.h>
	#include<string.h>
#endif


#include"diff.h"

using namespace std;


bool debugging;

void errorHelp(int errC, char *msg)
{
	if(errC == 0){
		printf("\nVCD Diff\nFinds the difference between two Verilog Simulation VCD files\n\n");
		printf("Usage: VCD <file1> <file2> [-s] [-v] [-m Module_Name] [-r Reference_Name] [-t Start_Time End_Time]\n\n");
		printf("Options\n");
		printf("   -s                       Show difference in scopes\n");
		printf("   -v                       Show difference in variable references\n");
		printf("   -m Module_Name           Only do comparision for signal variables within the specified module\n");
		printf("                            (Doesn't include signal variables in subscopes within the indicated scope)\n");
		printf("   -r Reference_Name        Only compare signals for the specified reference\n");
		printf("   -t Start_Time End_Time   Only do comparision within the specified time range\n");
		printf("\n   -h -help --help\n");
		printf("   /? -? ?                  Shows this help message\n");
		exit(0);
	}
	if(errC == 1){
		printf("\nError Opening file \" %s \nMake sure the file is in the directory\nor filename is typed correctly", msg);
		exit(1);
	}

}


bool is_scope(string Address, string ScopeName){
	int y;
	Address.erase(Address.size()-3, 3);
	if(!ScopeName.compare(Address.substr(y = Address.find_last_of(" ")+1, Address.size() - y)))
		return true;
	else
		return false;
}


clock_t st;

int main(int argc, char **argv)
{
	//arguments to be passed to diff from user inputs
	struct argvals{
		bool diff_scope, diff_var, spec_scope, spec_ref, spec_time;
		string ref_tocheck, scope_tocheck;
		int st_time, en_time;
		
		argvals(){
			diff_scope = diff_var = spec_scope = spec_ref = spec_time = 0;
			ref_tocheck = scope_tocheck = "";
			st_time = en_time = 0;
		}
	};


	st = clock();
	FILE *file;
	argvals argb;
	
	//extern bool debugging;
	
	debugging = false;
	string scope_name, ref_name;

	if(argc < 3 || argc > 12){
		errorHelp(0, NULL);
		return 0;
	}

	if(argc > 3 && argc <= 12){
		for(int i = 1; i < argc; i++){
			if(i == 1 || i == 2){
				if(!(file = fopen(argv[i], "r"))){
					errorHelp(1, argv[i]);
				}
				else fclose(file);
			}
			if(!strcmp(argv[i], "-s"))
				argb.diff_scope = true;
			if(!strcmp(argv[i], "-d"))
				debugging = true;
			if(!strcmp(argv[i], "-v"))
				argb.diff_var = true;
			if(!strcmp(argv[i], "-m")){
				argb.spec_scope = true;
				argb.scope_tocheck = scope_name = argv[i+1];
			}
			if(!strcmp(argv[i], "-r")){
				argb.spec_ref = true;
				argb.ref_tocheck = ref_name = argv[i+1];
			}
			if(!strcmp(argv[i], "-t")){
				argb.spec_time = true;
				if(strtol(argv[i+1], NULL, 10) < strtol(argv[i+2], NULL, 10)){
					argb.st_time = strtol(argv[i+1], NULL, 10);
					argb.en_time = strtol(argv[i+2], NULL, 10);
				}
				else
					printf("\nTime Range Error!\nStart time %s can't be greater or equal to end time %s", argv[i+1], argv[i+2]);
			}//end of if check for time range
		}//end of for loop
	}//end of if argc > 3





	//VCDparse file1(argv[1]), file2(argv[2]);

	//printf("file1\nnum of scopes %d num of vars %d", file1.num_scope, file1.num_var);
	//printf("file2\nnum of scopes %d num of vars %d", file2.num_scope, file2.num_var);
	diff VCDdiff(argv[1], argv[2]);
	cout<<"\n\ntime taken "<<((float)clock()/1000)<<" seconds\n\n";
	if(argb.spec_time){
		if(argb.st_time < VCDdiff.file1.st_time || argb.st_time < VCDdiff.file2.st_time ||
			argb.en_time > VCDdiff.file1.end_time || argb.en_time > VCDdiff.file2.end_time)
			printf("\nERROR WITH TIME RANGE\n\tFile: %s \n\t\t start time = %d\n\t\t end time = %d\n\tfile: %s \n\t\t start time = %d \n\t\t end time = %d\n",
				argv[1], VCDdiff.file1.st_time, VCDdiff.file1.end_time, argv[2], VCDdiff.file2.st_time, VCDdiff.file2.end_time);
	}
	if(argb.diff_scope)
		VCDdiff.CheckScopeNames();
	if(argb.diff_var)
		VCDdiff.CheckVarNames();

	if(argb.spec_ref){//if -r
		if(debugging) cout<<"\n----------In if -r--------\n";
		if(argb.spec_scope){//if -m
			if(debugging) cout<<"\n----------In if -r if -m--------\n";
			//checking if ref is in more than 1 scope
			map<string, map<string, string> >::const_iterator fnd = VCDdiff.file1.address_var.find(argb.ref_tocheck);
			//before going to see if in multi scope... first check if -m actually exists in one of the scopes
			if(fnd != VCDdiff.file1.address_var.end()){
				if(fnd->second.size() > 1){
					//check if -m is in more than one scope then if more than one,
					//show list of scopes the ref is in
					//else... just do diff
					if(debugging) cout<<"\n----------In if -r if -m and fnd->second > 1--------\n";
					map<string, string>::const_iterator additer = fnd->second.begin();
					int counter = 0;
					vector<string> sccheck;
					while(additer != fnd->second.end()){
						if(is_scope(additer->first, argb.scope_tocheck)){
							cout<<endl<<counter<<". "<<additer->first;
							sccheck.push_back(additer->first);
							++counter;
						}
						additer++;
					}
					if(counter == 1){
						//do diff
						if(debugging) cout<<"\n----------In if -r if -m and if counter == 1--------\n";
						if(debugging) cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@the ref is "<<fnd->first<<" and addrs = "<<sccheck[0]<<endl<<endl;
						VCDdiff.VarValueChange(fnd->first, sccheck[0]);
					}
					else if(counter == 0){
						//doesn't exist
						if(debugging) cout<<"\n----------In if -r if -m and if counter == 0--------\n";
						if(debugging) cout<<"Looks like the requested ref isn't found in the requested scope\n\n";
					}
					else{
						if(debugging) cout<<"\n----------In if -r if -m and if counter > 1--------\n";
						if(debugging) cout<<"i.e. the requested scope occurs multiple times and the ref name is also found repeatedly in the same scope_name\n";
						//use counter to accept and calc which scope usr wants
						int inpt, inctr = 0;
						//string sc_tocheck;
						inptprompt1:
						cout<<"\nWhich scope (number input) ";
						cin>>inpt;
						if(inpt >= sccheck.size() || inpt < 0){
							cout<<"\nERROR INPUT - VALUE SHOULD BE INTEGER FROM 0 TO "<<sccheck.size()-1;
							cin.clear();
							cin.ignore();
							goto inptprompt1;
						}
						cout<<endl<<endl<<"u chose "<<sccheck[inpt]<<endl;
						if(debugging) cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@the ref is "<<fnd->first<<" and addrs = "<<sccheck[inpt]<<endl<<endl;
						VCDdiff.VarValueChange(fnd->first, sccheck[inpt]);
						sccheck.clear();//sccheck isn't needed anymore
						//when the loop finishes... sc_tocheck is the chosen one;
					}
				}
				else{//if -r (chosen ref) has a single address (is located in one scope only)
					//before proceeding to diff first check if the reqqueseted scope actually contains the chosen ref
					if(debugging) cout<<"\n----------In if -r if -m and fnd->second !> 1--------\n";
					map<string, string>::const_iterator additer = fnd->second.begin();
					//checking if the single scope the ref is in is actually the usr requested scope
					if(!is_scope(additer->first, argb.scope_tocheck))
						cout<<"\nThe ref isn't in the chosen scope... it's in\n"<<additer->first<<"\nDoing diff for that\n";
					else
						cout<<"\nThe ref is found in the chosen scope... doing diff\n";
					if(debugging) cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@the ref is "<<fnd->first<<" and addrs = "<<additer->first<<endl<<endl;
					VCDdiff.VarValueChange(fnd->first, additer->first);
					//do diff for additer->first in either case
				}

			}
			else{//requested ref doesn't exist
					printf("\nERROR! THE REQUESTED REFERENCE NAME DOESN'T EXIST.\n CHECK SPELLING AND CASE\nExiting...");
					return 1;
				}
/////////////////////////////////////////////////////////////////////////////////////////////////////
		}//end of if -m
		else{//if no -m
			if(debugging) cout<<"\n----------In if -r if NO -m--------\n";
			map<string, map<string, string> >::const_iterator fnd = VCDdiff.file1.address_var.find(argb.ref_tocheck);
			if(fnd->second.size() > 1){
				//show list of scope the ref is in
				int ctr = 0;
				if(debugging) cout<<"\n----------In if -r if NO -m and fnd->second > 1--------\n";
				printf("\nThe Chosen Reference occurs in multiple scopes.\nThe following is a list of those scopes.\n\n");
				map<string, string>::const_iterator additer = fnd->second.begin();
				while(additer != fnd->second.end()){
					cout<<endl<<ctr<<". "<<additer->first;
					ctr++;
					additer++;
				}
				--ctr;
				int inpt;
				inptprompt2:
				cout<<"\nWhich scope (number input) ";
				cin>>inpt;
				if(inpt > ctr || inpt < 0){
					printf("\nINVALID INPUT!!! - Input should be integer b/n 0 and %d\n", ctr);
					//cout<<"\nINVALID INPUT!!! - Input should be integer b\/n 0 and "<<ctr<<endl;
					cin.clear();
					cin.ignore();
					goto inptprompt2;
				}
				additer = fnd->second.begin();
				for(ctr = 0; ctr != inpt; ctr++)
					++additer;
				cout<<"\nu chose scope "<<additer->first;
				if(debugging) cout<<"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@the ref is "<<fnd->first<<" and addrs = "<<additer->first<<endl<<endl;
				VCDdiff.VarValueChange(fnd->first, additer->first);
				//do diff for additer->first
			}
			else if(fnd->second.size() == 1){//if	 -r ins't in more than one scope without -m
				//do diff for additer->first
				map<string, string>::const_iterator additer = fnd->second.begin();
				if(debugging) cout<<"\n----------In if -r if NO -m and if fnd->second !> 1--------\n";
				if(debugging) cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@the ref is "<<fnd->first<<" and addrs = "<<additer->first<<endl<<endl;
				VCDdiff.VarValueChange(fnd->first, additer->first);
			}
			else{
				printf("\nERROR! THE REQUESTED REFERENCE NAME DOESN'T EXIST.\n CHECK SPELLING AND CASE\nExiting...");
				return 1;
			}
		}
	}//end of if -r
	else{//if no -r
		if(debugging) cout<<"\n----------In if NO -r--------\n";

		if(argb.spec_scope){//if -m
			if(debugging) cout<<"\n----------In if NO -r if -m--------\n";

			//idk how to check if scope name is multi
			//if not multi assemble signal and do diff but if multi first show list before diff
			map<string, vector<string> >::const_iterator scadd_it = VCDdiff.file1.scope_address.find(argb.scope_tocheck);
			if(scadd_it != VCDdiff.file1.scope_address.end()){//requested scope found
				//now u can check if it's multi
				if(debugging) cout<<"\n----------In if NO -r if -m and if requested scope is found--------\n";
				//string ch_add;
				vector<string>::const_iterator additer = scadd_it->second.begin();
				if(scadd_it->second.size() > 1){//if multi address
					if(debugging) cout<<"\n----------In if NO -r if -m and if scadd_it->second.size() > 1--------\n";

					int ctr = 0, inpt;
					cout<<"\nThe chosen scope name occurs multiple times\nPlease choose the right address...\n";
					while(additer != scadd_it->second.end()){
						cout<<ctr<<". "<<additer->data()<<endl;
						++ctr;
						++additer;
					}
					--ctr;
					cout<<"\n(number input) ";
					inptprompt3:
					cin>>inpt;
					if(inpt > ctr || inpt < 0){
						printf("\nINVALID INPUT!!! - Input should be integer b/n 0 and %d\n", ctr);
						//cout<<"\nINVALID INPUT!!! - Input should be integer b\/n 0 and "<<ctr<<endl;
						cin.clear();
						cin.ignore();
						goto inptprompt3;
					}
					additer = scadd_it->second.begin();
					additer += inpt;
					cout<<"\nyou have chosen "<<additer->data()<<endl;
				}
				else{//else isn't needed here it can be removed
					//it's not multi
					if(debugging) cout<<"\n----------In if NO -r if -m AND -m isn't multi--------\n";
					//ch_add = additer->data();
					cout<<"\nThe single address is "<<additer->data();
				}
				//aadd the iterating code to call varvaluechange here
				//and create a sstring to hold the chosen address in both cases...that string is ch_add
				map<string, map<string, string> >::const_iterator refaddr_iter = VCDdiff.file1.address_var.begin();
				while(refaddr_iter != VCDdiff.file1.address_var.end()){
					map<string, string>::const_iterator addvar_iter = refaddr_iter->second.begin();
					while(addvar_iter != refaddr_iter->second.end()){
						//compare the chosen address with the address from addvar_iter and if match... diff!!
						if(!addvar_iter->first.compare(additer->data())){
							cout<<"\n"<<refaddr_iter->first;//do diff
							VCDdiff.VarValueChange(refaddr_iter->first, additer->data());
						}
						addvar_iter++;
					}
					refaddr_iter++;
				}
			}
			else{
				//requensted scope not found
				if(debugging) cout<<"\n----------In if NO -r if -m BUT requested scope isn't found--------\n";
				cout<<"\nTHE REQUESTED SCOPE ISN'T FOUND!!!\nEXITING...";
				return 1;//error cuz scope not found
			}
		}
		else{//if no -m
			if(debugging) cout<<"\n----------In if NO -r AND if NO -m--------\n";

			//warn usr of the proble with doing an all-diff
			//and continue with usr choice
			cout<<"\nCHECKING ALL VARIABLES TAKES A VERY LONG TIME\nARE YOU SURE YOU WANT TO DO THIS (Y/N)\n";
			if(tolower(getchar()) == 'y')
				;
			else{
				cout<<"\nGood Choice!\nIt's better to select a specific scope or reference for comparision\nJust lookin out for u :)";
				return 0;
			}
			//get rid of the upper similar code
		}
	}

	if (debugging) VCDdiff.varvalchange(argv[3]);

/*	VCDdiff.spec_ref = spec_ref;
	VCDdiff.spec_scope = spec_scope;
	VCDdiff.spec_time = spec_time;
	VCDdiff.diff_scope = diff_scope;
	VCDdiff.diff_var = diff_var;	*/

	if(debugging) cout<<endl<<endl
		<<"spec_ref "<<argb.spec_ref<<endl
		<<"spec_scope "<<argb.spec_scope<<endl
		<<"spec_time "<<argb.spec_time<<endl
		<<"diff_scope "<<argb.diff_scope<<endl
		<<"diff_var "<<argb.diff_var<<endl;
	cout<<"\n\ntime taken "<<((float)clock()/1000)<<" seconds";
return 0;
}
