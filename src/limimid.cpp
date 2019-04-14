/*
	VCD diff program
		finds the difference between two verilog VCD simulation files
	Tuesday, ‎June ‎30, ‎2015, ‏‎12:42:36 AM

differences to note
					-number of scopes and variables
					-scope and variable reference names
					-variable values from value change section
*/


#include<iostream>
#include<fstream>
#include<Windows.h>
#include<string>
#include<algorithm>
#include<vector>
#include<map>
#include"ValChange.h"

using namespace std;

int num_scope, num_upscope, num_vars;
int *scope_pos, *upscope_pos, *var_pos;

struct varman{
	string type;
	int size;
	string id_code;
	string refnobit;
	string reference;
	string parent_name;
	string address;
};




varman *variable_man;



void forDebug();	//prototype for debugging function*************remove when done with



size_t GetFSize(FILE *file)
{
	size_t LastByte;
	fseek(file, 0, SEEK_END);
	LastByte = ftell(file);  
	fseek(file, 0, SEEK_SET);
	return LastByte;
}

size_t CountOccur(string str, string str2)
{
	int i = 0, f = str.find(str2);
	while(f != -1)
	{
		f = str.find(str2, f+str2.length());
		i++;
	}
return i;
}


int getEndDefPos(char * filename)//get the position of $enddefintions
{
	ifstream f;
	string temp = "";
	f.open(filename, ios::binary);//used binary mode so that no bit convesions are made
	//i'm not gonna be stuck on this for another two days so DON'T TOUCH IT!!!!!!!!!!!!!!!!!!!!!!!!!
	while((temp.find("$enddefinitions")) == -1){
		f >> temp;
	}
	int position = (int)f.tellg() - temp.size();
	f.close();
	return position;
}


void varManager(int currentpos)
{
	//ere do somethin here!!! gud eko naw!
}


void Positions(string str, string str2, int *pos)
{
	int offset = str.find(str2);
	int n = CountOccur(str, str2);
	pos = new int [n];
	for(int i = 0; offset != -1; i++)
	{
		pos[i] = offset;
		offset = str.find(str2, offset+1);
	}
}



void main(int argc, char * argv[])
{
	if (argc == 1) //exit if program opened with no arguments
		exit(0);
	
	
	char *scope = "$scope", *end = "$end", *upscope = "$upscope", 
		 *var = "$var", *enddef = "$enddefinitions", *comment = "$comment",
		 *date = "$date", *timescale = "$timescale", *version = "$version", 
		 *dumpvars = "$dumpvars", *dumpoff = "$dumpoff", *dumpon = "$dumpon",
		 *dumpall = "$dumpall";
	
	string temp;
	char * tempstr;
	int enddef_pos = 301886;

	//printf("Position of $enddefinitions = %d\n", enddef_pos);

	FILE *file;
	char *buffer;
	ifstream file2;

	


	file = fopen(argv[1], "r");
	printf("\nFile size = %d\n", GetFSize(file));

	enddef_pos = getEndDefPos(argv[1]);
	cout<<"\nPosition of $enddefinitions "<<enddef_pos<<endl;
	//set buffer size to just upto $enddefinitions
	//put in buffer the contents of the file upto $enddefinitions
	buffer = new char[enddef_pos];
	fread(buffer, enddef_pos, 1, file);

	
	string hey = buffer;
	


		num_scope = CountOccur(hey, scope);
		num_upscope = CountOccur(hey, upscope); 
		num_vars = CountOccur(hey, var);
		printf("\n\nNumber of occur of scope %d\n", num_scope);
		printf("Number of occur of upscope %d\n", num_upscope);
		printf("Number of occur of variables %d\n", num_vars);
	
	if (num_scope != num_upscope) {//there must be the same number of $scope and $upscope keywords
		cerr<<"Error with scope defintions!\nExiting...";
		system("pause>nul");
		exit(1);
	}



	

	
	variable_man = new varman [num_vars];

	COORD pos;
	pos.X = 0; pos.Y = 13;
	int i =  0;
	int offset = 0;
	
	scope_pos = new int [num_scope];
	upscope_pos = new int [num_upscope];
	var_pos = new int[num_vars];
	int scope_offset =hey.find(scope), 
		upscope_offset = hey.find(upscope),
		var_offset = hey.find(var);

	for(int k = 0; scope_offset != -1; k++)//loop setting locations of scope
	{
		scope_pos[k] = scope_offset;
		scope_offset = hey.find(scope, scope_offset+1);
	}

	for (int k = 0; upscope_offset != -1; k++)
	{
		upscope_pos[k] = upscope_offset;
		upscope_offset = hey.find(upscope, upscope_offset+1);
	}

	for (int k = 0; var_offset != -1; k++)
	{
		var_pos[k] = var_offset;
		var_offset = hey.find(var, var_offset+1);
	}

	
	fclose(file);

	
	//	forDebug();


	//********************************************************************************************


	file2.open(argv[1]);
	
	vector<string> definitions;
	map<string, int> counter;
	file2 >> temp;
	i = 0;
	///////////////////////////////////////////
	//parallel values
	//////////////////////////////////////////
	vector<string> scopeID, scopes_LIFO;
	vector<int> scopedepth;
	int depth = 0;
	string scope_type, scope_name;
	/////////////////////////////////////////
	int var_count = 0;

	while(temp.compare("$enddefinitions"))
	{
		if(!temp.compare(scope) || !temp.compare(upscope) || !temp.compare(var))
		{

			
			if (!temp.compare(scope)){  
	//			cout<<"just after scope "<<file2.tellg()<<" ";
			//	printf("just after scope %d ", (int)file2.tellg());
				
				pos.Y++;
				definitions.push_back(temp);
				depth++;
				pos.X += 5;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
				file2 >> scope_type >> scope_name;
			//	printf("%s ", scope_type);
				
				
				
				scope_type.append(" ");
				scope_type.append(scope_name);
				////////////////////////////////////////////////
				scopedepth.push_back(depth);
				scopeID.push_back(scope_type);
				cout<<scope_type<<" "<<depth;
				++counter[scope_name];


				scopes_LIFO.push_back(scope_type);

			//	printf("%s %d", scope_type, depth);
				
			//	varManager(file2.tellg());

/*
	maybe i need to store all variable info on a certain container and 
	then choose a var from the first file, look for that var in the second file
	and then go to the value change section working out the differences


*/
//#################################################################################
				////////////////////////////////////////////
				//cout<<scopeID[i]<<" "<<scopedepth[i]<<" "<<"scope_pos = "<<scope_pos[i]<<" tellg "<<(int)file2.tellg();
				//printf("%s %d scope_pos = %d tellg %d ", (string)scopeID[i], (int)scopedepth[i], (int)scope_pos[i], (int)file2.tellg());
				i++;
				/*
					vectors scopedepth and scopeID are parallel and 
					scopeID holds the scope identifier and scopedepth holds that
					scope's depth in the hierarchy
				*/
			}
			if (!temp.compare(upscope)){
		//		pos.Y++;
				definitions.push_back(temp);
				scopes_LIFO.pop_back();
				depth--;
				pos.X -= 5;
				//printf(" upscope_pos %d ", upscope_pos[i]);
			}

			string templine;
			if(!temp.compare(var)){
				
/*
	the following code is executed when the $var keyword is found
	what it does is it uses getline(file, buffer) just after $var so
	the following characters should always be 
	" <var_type> <size> <id_code> <reference> <$end>"
	so it tokenizes the line by using space as a delimiter
		first it gets the positions of all the spaces in the line
		since there could be multiple spaces, i couldn't relay on
		just looking for a space and assume the next char is the beggining
		of one of the <>s 
		after getting the positions of the spaces, it uses the values to 
		extract substrings from the string templine obtained by getline(file2, templine)
	by using Count, the substrings are set to their appropriate places in the variable_man
	structure i.e. if Count is 0 then it's the first substring which is var_type
	if Count = 1 then the second substring is size and so on and on
	the only special thing is on Count = 4, it's suppose to be just the name of the reference but
	after that that reference may have a bit-select indicator and so it compares the substring
	to $end and if it's not the same then it means it's a bit-select identifier so it concatenates it
	with string refrence in variable_man with a space in between (just like how it is in the file)
	and on count=3, the refnobit string holds the reference name without the bitselect... to make
	it easier when searching for vars that affect the same references... the bit select may have 
	different values but the reference name is the same so do the search using refnobit
	or maybe create a string in varman that hods only the bit-select

*/

				
				getline(file2 , templine); //templine stores line after $var
				//have a tokenizer that parses the line into vartype, size, idcode and reference
				//untill $end is found -- it should be like a function
				 
		//		cout<<"\ntempline \n"<<templine<<endl<<"012345678901234567890123456789"<<endl; 
				//##############################################################
				string tempword, type_v, size_v, code_v, ref_v, bit_v, end_v;
				
				int temp_index = 0, j, fpos = 0, lenn = 0, len, Count = 0;

				int *space_pos;
				int space_off = templine.find(" ");
				
				space_pos = new int [CountOccur(templine, " ")];
				for(int jjj = 0; space_off != -1; jjj++)
				{
					space_pos[jjj] = space_off;
					space_off = templine.find(" ", space_off+1);
				}
			//	for(int y = 0; y < CountOccur(templine, " "); y++)
				//	cout<<space_pos[y]<<endl;
				for(int y = 0, Count = 0; y < CountOccur(templine, " ")-1; y++){
					fpos = space_pos[y] + 1;
					lenn = (space_pos[y+1] - space_pos[y])-1;
					if(lenn != 0){
						if(Count == 0)
							variable_man[var_count].type = templine.substr(fpos, lenn);
						if(Count == 1)
							variable_man[var_count].size = stoi(templine.substr(fpos, lenn), nullptr, 10);
						if(Count == 2)
							variable_man[var_count].id_code = templine.substr(fpos, lenn);
						if(Count == 3){
							variable_man[var_count].reference = templine.substr(fpos, lenn);
							variable_man[var_count].refnobit = templine.substr(fpos, lenn);
						}
						if(Count == 4 && templine.substr(fpos, lenn).compare(end)){
							variable_man[var_count].reference.append(" "); 
							variable_man[var_count].reference.append(templine.substr(fpos, lenn));
						}
						
						variable_man[var_count].parent_name = scopeID.back();
						Count++;
					}
				}
				
				string address = "", lifo_address;


				vector<string>::const_reverse_iterator vecit = scopes_LIFO.rbegin();

				while(vecit != scopes_LIFO.rend()){
					lifo_address.insert(0, vecit->c_str());
					lifo_address.insert(0, " / ");
					vecit++;
				}

			//	cout<<endl<<endl<<"-------------- "<<lifo_address<<" ------------ "<<variable_man[var_count].id_code<<endl<<endl;

				/*
					the following creates an address for a var
					the address starts with the topmost scope and continues with subscopes until
					the scope which contains the var
					has format parent_scope/sub_scope/sub_scope/...
					it's stored in address under the varman structure
				*/
				int prev = 0, curdep = scopedepth.back();
				for(int l = scopedepth.size() - 1;;l--){//loop looking for parent scopes and create an address string
					if(scopedepth[l] == curdep){
						if(scopedepth[l] == 1){
							address.insert(0, scopeID[l]);
							address.insert(scopeID[l].size(), " / ");
							break;
						}
						else{
							address.insert(0, scopeID[l]);
							address.insert(scopeID[l].size(), " / ");
							curdep--;
						}
						prev = scopedepth[l];
					}
					
				}
			//	cout<<"\n  ********************** "<<address<<" *** "<<variable_man[var_count].reference<<"\n";
				variable_man[var_count].address = lifo_address;
				var_count++;
				delete [] space_pos;
				//##############################################################
	
				



/*
				//		cout<<"here111111111111111111111111111111111111111111111111111";
				file2 >> temp;
				variable_man[var_count].type = temp;
				//cout<<"  var type = "<<variable_man[var_count].type;
				file2 >> temp;
				variable_man[var_count].size = stoi(temp, nullptr, 10);
			//	cout<<"  var size = "<<variable_man[var_count].size;
				file2 >> temp;
				variable_man[var_count].id_code = temp;
		//		cout<<"  var id_code = "<<variable_man[var_count].id_code;
				file2 >> temp;
				variable_man[var_count].reference = temp;
	//			cout<<"  var ref = "<<variable_man[var_count].reference;
				variable_man[var_count].parent_name = scopeID[i-1];
				//cout<<"  var parent = "<<variable_man[var_count].parent_name;
				var_count++;
				//cout<<endl;*/
				//system("pause>nul");
			}
			
		}
			
		file2 >> temp;
	}
	printf("\n\nlooped for %d times", var_count);

	typedef map<string, vector<string>>::value_type valt;
	typedef pair<map<string, int>, bool> returned;
	map<string, vector<string>> ref_man, parent_ref_var, address_var;
	map<string, string> var_ref_man;
	vector<string> tempvector;
	cout<<endl<<num_vars<<endl;
	string STR;
	for(int uu = 0; uu < num_vars; uu++){
		ref_man[variable_man[uu].refnobit].push_back(variable_man[uu].id_code);
		var_ref_man[variable_man[uu].id_code] = variable_man[uu].reference;
		STR = variable_man[uu].parent_name;
		STR.append(" ");
		STR.append(variable_man[uu].reference);
		parent_ref_var[STR].push_back(variable_man[uu].id_code);
		address_var[variable_man[uu].address].push_back(variable_man[uu].id_code);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	forDebug();
	int trial = 1;
	cout<<"\n\nNum of Unique References = "<<ref_man.size()<<endl;
	map<string, vector<string>>::const_iterator it = parent_ref_var.begin();
	map<string, vector<string>>::const_iterator addit = address_var.begin();
	while(trial != 0)
	{
		cout<<"\nEnter index number ";
		cin>>trial;
		cout<<endl<<"Trial........................."<<endl;
		cout<<"type = "<<variable_man[trial].type<<endl;
		cout<<"size = "<<variable_man[trial].size<<endl;
		cout<<"idcode = "<<variable_man[trial].id_code<<endl;
		cout<<"refnobit = "<<variable_man[trial].refnobit<<endl;
		cout<<"reference = "<<variable_man[trial].reference<<endl;
		cout<<"parent = "<<variable_man[trial].address<<endl<<endl;
		cout<<"\nvar  "<<variable_man[trial].id_code<<" has references"<<endl;
		for(int uu = 0; uu < ref_man[variable_man[trial].refnobit].size(); uu++){
			//cout<<ref_man[variable_man[trial].refnobit][uu]<<endl;
			cout<<var_ref_man[variable_man[trial].id_code]<<endl;
		}
		cout<<"\n\n------------------------------------------------\n\n";
		
		
		for(int uu = 0; uu < 10; uu++, addit++)
			for(int UU = 0; UU < it->second.size(); UU++)
				cout<<addit->first<<" ------- "<<addit->second[UU]<<endl;
	}
	/*
		i just found duplicate var id_codes when running this... maybe it's okay...idk
		but since there's no duplicate checking before pushing into the vector i.e
		the second member in ref_man... there're the same vars stored repeatedly in the
		vector... doesn't affect the performance that much but it's stupid so avoid deipulicate
		storage or know why it's happening and modify the code to account for it
	*/
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//gonna do the val change here
	cout<<"\nllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll\nposition of enddef = "<<enddef_pos;
	cout<<endl<<"hey at enddef = "<<hey[enddef_pos+1]<<endl;
	file2.seekg(enddef_pos);

	string valchange;
	getline(file2, valchange);
	cout<<"\n"<<valchange<<endl;
	//line after this getline is either $dumpvars or the value change section begines
	vector<int> time;
	getline(file2, valchange);
	if(!valchange.compare(dumpvars)){
		time.push_back(0);
	}
	char begin;
	int length = 0, sp_pos;
	string var_code, value;
	trial = 1;
	while(trial != 0){
		cin>>trial;
		getline(file2, valchange);
		//parsing line
		begin = valchange[0];
		if(begin == '0' || begin == '1' || tolower(begin) == 'x' || tolower(begin) == 'z'){
			//then next chars are varcode
			length = valchange.size();
			var_code = valchange.substr(1, length);
			cout<<endl<<"state change -> "<<var_code<<" "<<valchange[0]<<"  at time "<<time.back()<<endl;
		}
		if((tolower(begin) == 'b' || tolower(begin) == 'r') && (valchange.find(" ") != -1)){
			sp_pos = valchange.find(" ");
			length = valchange.size();
			value = valchange.substr(1, sp_pos-1);
			var_code = valchange.substr(sp_pos+1, (length - 1) - (value.size() + 1));
			cout<<endl<<"value change "<<var_code<<" = "<<value<<"  type = "<<valchange[0]<<"  at time "<<time.back()<<endl;
		}
		if(valchange[0] == '#'){
			time.push_back(stoi(valchange.substr(1, valchange.size() - 1), nullptr, 10));
		}
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	file2.close();

	ValChange ff(enddef_pos, argv[1]);
	map<string, vector<string>>::const_iterator itt = parent_ref_var.begin();
	cout<<endl<<endl;
	vector<string> varsof(itt->second);
	for(int b = 0; b < 10; b++){
	cout<<"\nVarsof = "<<itt->first<<"\n";
	for(int k = 0; k < varsof.size(); k++)
		cout<<varsof[k]<<endl;
	itt++;
	}
	cout<<"\n\n00000000000000000000000000000000000000000000000000000000000\n\n";
	ff.reset();
	
	while(!ff.end){
		
		for(int e = 0; e < varsof.size(); e++){
			if(!ff.var_code.compare(varsof[e])){
				cout<<"change type = "<<ff.change_type<<endl;
				cout<<"varcode = "<<ff.var_code<<endl;
				cout<<"value = "<<ff.value<<endl;
				cout<<"time = "<<ff.time<<endl<<endl;
				ff.getNextLine();
				break;
			}
			else
				ff.getNextLine();
		}
	}

	cout<<"\n\n1111111111111111111111111111111111111111111111111111111\n\n";
	
	cout<<"Scopes that occure more than once...\n\n";
	map<string, int>::const_iterator ita = counter.begin();
	while(ita != counter.end())
	{
		if(ita->second > 1)
			cout<<ita->first<<" "<<ita->second<<endl;
		++ita;
	}
	
//commented out the following code b/c my first concept of value change was wrong
//won't be back for it
/*	//####################################################################################
	//gonna go through val change   xxxx xxx
	ifstream inf(argv[1]);
	cout<<"\n\n"<<getEndDefPos(argv[1])<<endl;
	inf.seekg(getEndDefPos(argv[1]) - 100);
	string curr_scope, idcode, line, val, varcode;
	int mid, len_val, len_var;
	getline(inf, val);
	cout<<endl<<val<<endl;
	int in = 0;
	curr_scope = variable_man[in].parent_name;
	vector<string> varvalues, time;

	while(!curr_scope.compare(variable_man[in].parent_name))
	{
		idcode = variable_man[in].id_code;
		cout<<"\nLooking for var "<<idcode<<" ref = "<<variable_man[in].reference<<" in "<<curr_scope<<endl;

		while(getline(inf, line))
		{
			cout<<endl<<line<<endl; 
			mid = line.find(" ");
			if(mid != -1)
			{
				len_var = line.size() - mid + 1;
				len_val = line.size() - len_var + 1;
				val = line.substr(0, len_val);
				varcode = line.substr(mid+1, len_var);
			}
		//	/*
				find space in line
				if space found
					copy str before space to val and after space to varcode
					compare varcode to idcode
					if same
						push val into vector varvalues
						
				if space not found
					check if first char is state char or time indicator
					if state char
						compare varcode to idcode
						if same
							push val into vector varvalues
						else
							goto next
					if time indicator
						push val (value after #) into time
			
		}
		cout<<"\n\n*************val changes of "<<variable_man[in].id_code<<" are";
	//	printf("\n\n*************val changes of %s are", variable_man[in].id_code);
		//for(int sup = 0; sup < varvalues.size(); sup++)
			//cout<<varvalues[sup]<<endl;
		system("pause>nul");
		break;
		in++;
	}
	*/
	//####################################################################################



	/*for (int j = 0; j < definitions.size(); j++)
	{
		cout<<definitions[j]<<endl;
	}*/

	forDebug();




system("pause>nul");


endprog:;
	delete [] buffer, scope_pos, upscope_pos, var_pos, variable_man;
	system("pause>nul");
}






/*
**************************************************************
		JUST FOR DEBUGGING
**************************************************************
*/
void forDebug()
{
	//print 
		//the number and positions of scopes, upscopes and vars.
	ofstream debugfile("debugging file.txt");
	debugfile << "*********NUMBER OF OCCURENCES**************\n\n";
	debugfile << "Number of Scope definitions = "<<num_scope<<endl;
	debugfile << "Number of Upscope definitions = "<<num_upscope<<endl;
	debugfile << "Number of Variables = "<<num_vars<<endl<<endl;
	debugfile << "\n\n\n*********SCOPE POSITIONS**************\n\n";
	for(int kl = 0; kl < num_scope; kl++)
		debugfile << "\n"<<kl<<". $scope at "<<scope_pos[kl];

	debugfile << "\n\n\n*********UPCSOPE POSITIONS**************\n\n";
	for(int kl = 0; kl < num_upscope; kl++)
		debugfile << "\n"<<kl<<". $supcope at "<<upscope_pos[kl];

	debugfile << "\n\n\n*********VAR POSITIONS**************\n\n";
	for(int kl = 0; kl < num_vars; kl++)
		debugfile << "\n"<<kl<<". $var at "<<var_pos[kl];

	debugfile << "\n\n *********** VARIABLES  *****************\n\n";
	debugfile << "var_type \t\t size \t\t id_code \t\t reference \t\t parent\n";
	for (int i = 0; i < num_vars; i++)
		debugfile << variable_man[i].type << " \t\t " << variable_man[i].size 
				  << " \t\t " << variable_man[i].id_code << " \t\t " << variable_man[i].reference 
				  << " \t\t " << variable_man[i].parent_name<<endl; 



	debugfile.close();
}