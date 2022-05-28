#include "util.h"
#include<vector>
#include<string>
using namespace std;

vector<string> split(string str, string delimiter)
{
	vector<string> internal;
	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		internal.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	internal.push_back(str);
	return internal;
}