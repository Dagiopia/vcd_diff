#include"VCDparse.h"
#include"g_func.h"

int VCDparse::getEndDefPos(char *filename)
{
	ifstream f;
	string temp = "";
	f.open(filename, ios::binary);
	while((temp.find("$enddefinitions")) == -1){
		f >> temp;
	}
	int position = (int)f.tellg() - temp.size();
	f.close();
	return position;
}



size_t VCDparse::GetFSize(FILE *file)
{
	size_t LastByte;
	fseek(file, 0, SEEK_END);
	LastByte = ftell(file);
	fseek(file, 0, SEEK_SET);
return LastByte;
}



int VCDparse::Definitions_section(char *filename)
{

	FILE *file = fopen(filename, "r");
	if(debugging) cout<<"\nin def section before get\n\n";
	int end_def = getEndDefPos(filename);
	if(debugging) cout<<"\nin def section\n\n"<<end_def<<endl<<endl;
	buffer  = new char [end_def];
	fread(buffer, end_def, 1, file);
	fclose(file);
	if(debugging) cout<<"\nexiting enddef\n\n";
return end_def;
}


void VCDparse::varManager(char *filename)
{
	ifstream infile(filename);
	vector<string> scope_man;//lifo for scope managment in creating the address
	//i couldn't use stack for operating with lifo cuz stack doesn't allow iteration as far as i know
	string temp, templine;
	depth = 0;
	index = 0;
	var_count = 0;
	int fpos = 0, length = 0, count = 0;
	if(debugging) cout<<"\n\nin var manager\nbefore anything\n";
	infile >> temp;
	while(temp.compare(enddef)){
		if(!temp.compare(timescale)){
			infile >> temp >> time_unit;
			time_scale = strtol(temp.c_str(), NULL, 10);
		}
		if (!temp.compare(scope) ||	!temp.compare(upscope) || !temp.compare(var)){
			if (!temp.compare(scope)){
			//found keyword $scope thus the next strings are <scope_type> <scope_name>
				definitions.push_back(temp);
				depth++;
				infile >> scope_type >> scope_name;
				scope_type.append(" ");
				scope_type.append(scope_name);
				scopedepth.push_back(depth);
				scopeID.push_back(scope_type);
				scope_man.push_back(scope_type);
				index++;
			}

			if(!temp.compare(upscope)){
				definitions.push_back(temp); //i think i only used this for debugging... make sure ena remove it
				scope_man.pop_back();
				depth--;
			}

			if(!temp.compare(var)){
				int *space_pos, space_off;
				getline(infile , templine);
				try{
					space_pos = new int [CountOccur(templine, " ")];
				}catch(bad_alloc xa){
					cout<<"\nbad allocation!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
					delete [] buffer;
					//delete [] scope_pos;
					//delete [] upscope_pos;
					//delete [] var_pos;
					delete [] variable_man;
					exit(1);
				}

				space_off = templine.find(" ");
				for(int i = 0; space_off != -1; i++){
					space_pos[i] = space_off;
					space_off = templine.find(" ", space_off+1);
				}

				for(int in_var = 0, count = 0; in_var < CountOccur(templine, " ")-1; in_var++){
					fpos = space_pos[in_var] + 1;
					length = (space_pos[in_var+1] - space_pos[in_var])-1;
					if(length != 0){
						if(count == 0)
							variable_man[var_count].type = templine.substr(fpos, length);
						if(count == 1){
							char *ttnline;
							for(int ind = fpos; ind <= templine.size(); ind++){
                                ttnline[ind-fpos] = templine[fpos];
							}
							//ttnline = templine.substr(fpos, length)
							variable_man[var_count].size = strtol(ttnline, NULL, 10);
                        }
						if(count == 2)
							variable_man[var_count].id_code = templine.substr(fpos, length);
						if(count == 3){
							variable_man[var_count].reference = templine.substr(fpos, length);
							variable_man[var_count].refnobit = templine.substr(fpos, length);
						}
						if(count == 4 && templine.substr(fpos, length).compare(end)){
							variable_man[var_count].reference.append(" ");
							variable_man[var_count].reference.append(templine.substr(fpos, length));
						}

						variable_man[var_count].parent_name = scopeID[index-1];
						count++;

					}
				}
				string address = "", addrs = "";

				vector<string>::const_reverse_iterator addman_it = scope_man.rbegin();
				while(addman_it != scope_man.rend()){
					addrs.insert(0, " / ");
					addrs.insert(0, addman_it->c_str());
					addman_it++;
				}

				//addrs.erase(addrs.size()-3, 3);
				variable_man[var_count].address = addrs;
				var_count++;
				delete [] space_pos;
			}

		}
		infile >> temp;
	}
	get_startend_time(&infile);
	infile.close();
}

void VCDparse::get_startend_time(ifstream *infile)
{
	string Vline;
	bool first = true;
	while(getline(*infile, Vline)){//if not eof
		if(Vline.find("$dumpvars") != -1 && first){//if dumpvars is found before the first #time token the start time is zero
			st_time = 0;
			first = false;
		}
		if(Vline[0] == '#'){
			if(first){
                char *tvline;
                for(int ind = 1; ind < Vline.size(); ind++)
                    tvline[ind-1] = Vline[ind];
                //Vline.substr(1, Vline.size() - 1)
				st_time = strtol(tvline, NULL, 10);
				first = false;
			}
			else{
                char *tvline;
                for(int ind = 1; ind < Vline.size(); ind++)
                    tvline[ind-1] = Vline[ind];
                //Vline.substr(1, Vline.size() - 1)
				end_time = strtol(tvline, NULL, 10);
            }
		}
	}
	//REMOVE THE FOLLOWING WHEN DONE!
	if(debugging) cout<<endl<<endl<<endl<<"start time = "<<st_time<<endl<<"end time = "<<end_time<<endl<<endl;
}



void VCDparse::refManager()
{
//	string STR;
	for(int INDEX = 0; INDEX < num_var; INDEX++){
//		ref_man[variable_man[INDEX].refnobit].push_back(variable_man[INDEX].id_code);
//		var_ref_man[variable_man[INDEX].id_code] = variable_man[INDEX].reference;

		address_var[variable_man[INDEX].reference][variable_man[INDEX].address] = variable_man[INDEX].id_code;


//		STR = variable_man[INDEX].parent_name;
//		STR.append(" ");
//		STR.append(variable_man[INDEX].reference);
//		parent_ref_var[STR].push_back(variable_man[INDEX].id_code);
//		cout<<endl<<variable_man[INDEX].address<<"   "<<variable_man[INDEX].reference<<endl;

		//filling scope_address with values
		string scp_name = variable_man[INDEX].parent_name.substr(variable_man[INDEX].parent_name.find(" ")+1, variable_man[INDEX].parent_name.size());//just removing the scope type and leaving the name only
		map<string, vector<string> >::iterator scpad_it = scope_address.find(scp_name);
		if(scpad_it != scope_address.end()){//exists
			if(!Does_exist(scpad_it->second, variable_man[INDEX].address))
				scpad_it->second.push_back(variable_man[INDEX].address);
		}
		else//add new
			scope_address[scp_name].push_back(variable_man[INDEX].address);



	}

	//the following code prints the contents of scope_address
	//testing it contains what it's suppose to contain
	//REMOVE WHEN DONE
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*	cout<<endl<<endl<<scope_address.size();
	map<string, vector<string>>::const_iterator iyt = scope_address.begin();
	while(iyt != scope_address.end()){
		cout<<endl<<iyt->first;
		for(int o = 0; o < iyt->second.size(); o++)
			cout<<"\n\t"<<iyt->second[o];
		iyt++;
	}


	//the following code prints the contents of address_var
	//testing it contains what it's suppose to contain
	//REMOVE WHEN DONE

	map<string, map<string, vector<string>>>::const_iterator jk = address_var.begin();
	while(jk != address_var.end()){
		map<string, vector<string>>::const_iterator kl = jk->second.begin();
		cout<<"ref   "<<jk->first<<"-------------------------------------------------------------------------------------------\n";
		while(kl != jk->second.end()){
			cout<<"address \t"<<kl->first;
			for(int lk = 0; lk < kl->second.size(); lk++)
				cout<<"\nvar  "<<kl->second[lk]<<"       "<<kl->second.size();
			cout<<endl<<endl;
			kl++;
		}
		jk++;
	}*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}//end of refManager()


bool VCDparse::ischild(string child, string parent)
{
	if(child.find(parent) == -1)
		return false;
	else
		return false;
	//cout<<endl<<"------------------------------\nis\n"<<child<<endl<<"child of \n"<<parent<<endl<<ret<<endl<<endl;
}





void VCDparse::setup(char *filename)
{
	strcpy(scope, "$scope"); strcpy(end, "$end"); strcpy(upscope, "$upscope");
	strcpy(var, "$var"); strcpy(enddef, "$enddefinitions"); strcpy(comment, "$comment");
	strcpy(date, "$date"); strcpy(timescale, "$timescale"); 
	strcpy(version, "$version"); strcpy(dumpvars, "$dumpvars"); 
	strcpy(dumpoff, "$dumpoff"); strcpy(dumpon, "$dumpon"); 
	strcpy(dumpall, "$dumpall");

	if(debugging) cout<<"\nGetting the definitions section\n";

		enddef_pos = Definitions_section(filename);
		if(debugging) cout<<"\nPosition of $enddefinitions = "<<enddef_pos<<"\n";

		num_scope = CountOccur(buffer, scope);
		num_upscope = CountOccur(buffer, upscope);
		num_var = CountOccur(buffer, var);
		if(debugging) cout<<"\ndone getting number of keywords\nNumber of Scopes = "<<num_scope<<"\nNumber of Variables = "<<num_var<<"\n";

		if(debugging) cout<<"\ndone getting positions of keywords\n";
		variable_man = new varman[num_var];
		if(debugging) cout<<"\ncreated variable_man\n\n";
		varManager(filename);
		if(debugging) cout<<"\ndone with variable manager\n\n";
		refManager();
		if(debugging) cout<<"\nDone With reference manager\n\n";
		if(debugging) cout<<"\nexiting setup()... \n\n";
}

