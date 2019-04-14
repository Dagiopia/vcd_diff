#ifndef G_FUNC_H
#define G_FUNC_H
#include<vector>
#include<string>
#include<fstream>

static bool Does_exist(std::vector <std::string> vect, std::string Str)
{
	std::vector<std::string>::const_iterator fi = vect.begin();
	while(fi != vect.end()){ 
		if(!fi->compare(Str))
			return true;
		fi++;
	}
	return false;
}

static size_t CountOccur(std::string str, std::string str2)
{
	int i = 0, f = str.find(str2);
	while(f != -1)
	{
		f = str.find(str2, f+str2.length());
		i++;
	}
return i;
}


#endif
