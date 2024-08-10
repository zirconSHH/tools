#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool starts_with(const string &str, const string &start)
{
	int srclen = str.size();
	int startlen = start.size();

	if (srclen < startlen)
		return false;

	string temp = str.substr(0, startlen);

	return temp == start;
}

bool ends_with(const string &str, const string &end)
{
	int srclen = str.size();
	int endlen = end.size();

	if (srclen < endlen)
		return false;

	string temp = str.substr(srclen - endlen, endlen);

	return temp == end;
}