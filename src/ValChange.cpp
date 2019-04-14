#include"ValChange.h"
#include<stdlib.h>
void ValChange::getNextLine()
{
	int length, space_pos, timectr = 0;
start:
	if(getline(vcdfile, nline)){//if not eof

		end = false;

		if(nline.size() < 2)//skip line if it's one char or less
		{
			//cout<<"-----------------------------------"<<nline<<endl;
			//getline(vcdfile, nline);
			goto start;
		}
		if(nline.find("$enddefinitions") != -1 || nline.find("$end") != -1){
			//getline(vcdfile, nline);//skip the line
			goto start;
		}
		if(!nline.compare("$dumpvars") ||
			!nline.compare("$dumpoff") ||
			!nline.compare("$dumpon")){
			//getline(vcdfile, nline);//skip the line and set time to zero
			goto start;
		}

		if(nline[0] == '#'){
            char *tnline;
            for(int ind = 1; ind <= nline.size(); ind++){
                tnline[ind-1] = nline[ind];
            }
            //string ttnline = nline.substr(1, nline.size() - 1);
            //tnline = ttnline.c_str();
			time = strtol(tnline, NULL, 10);
			//getline(vcdfile, nline);
			goto start;
		}
		begin = nline.at(0);
		//cout<<endl<<nline;
		if(begin == '0' || begin == '1' || tolower(begin) == 'x' || tolower(begin) == 'z'){
			change_type = 'S';	//S for state change
			length = nline.size();
			value = begin;
			var_code = nline.substr(1, length);
		}

		if((tolower(begin) == 'b' || tolower(begin) == 'r') && (nline.find(" ") != -1)){
			change_type = 'V'; //V for value change
			space_pos = nline.find(" ");
			length = nline.size();
			value = nline.substr(1, space_pos-1);
			var_code = nline.substr(space_pos+1, (length - 1) + (value.size() + 1));
		}
		//cout<<"    "<<var_code<<" = "<<value;
	}
	else //end of file
		end = true;
}

bool ValChange::iskeyword(string str)
{
	for(int i = 0; i < 6; i++){
		if(str.find(keywords[i]) != -1)
			return true;
	}
}
