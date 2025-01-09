#pragma once

#include <iostream>
#include <map>

using namespace std;

class enum_map
{
public:
	enum_map(const string& filePath, const string& enumName);

	int enum_map_compare(const enum_map& cmpMap);

private:
	string enum_name;
	map<string, string> kv_map;
};
	