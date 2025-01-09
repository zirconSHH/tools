#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <map>

#include "enum_check.hpp"

using namespace std;
using namespace filesystem;

template<class k, class v>
bool map_entry_exists(const map<k, v>& kv_map, const k& key, const v& value, pair<k, v>& previous)
{
	for (auto it : kv_map)
	{
		if (it.first == key || it.second == value)
		{
			previous = make_pair(it.first, it.second);
			return true;
		}
	}

	return false;
}

enum_map::enum_map(const string& filePath, const string& enumName): enum_name(enumName)
{
	path p(filePath);
	if (!exists(p))
	{
		cout << "path: " << p.root_path() << " not exists" << endl;
		return;
	}

	ifstream file(p.string());
	if (!file.is_open())
	{
		cout << "file: " << p.string() << " open failed" << endl;
		return;
	}

	string line;
	bool execute = false;
	while (getline(file, line)) {
		if (line.find("enum " + enum_name) != string::npos)
		{
			execute = true;
		}

		if (!execute)
		{
			continue;
		}

		regex pattern("\\s*(\\S*)\\s*=\\s*(\\S[^,]*)");
		smatch result;
		if (regex_search(line, result, pattern))
		{
			string key = result[1];
			string value = result[2];
			//cout << "IFO: find {" << key << " = " << value << "}" << endl; 
			pair<string, string> pre;
			if (map_entry_exists(this->kv_map, key, value, pre))
			{
				cout << "ERR: {" << key << "} = " << value << " already exists, previous {" << pre.first << "} = " << pre.second << endl;
			}
			else
			{
				this->kv_map[key] = value;
			}
		}

		if (execute && line.find('}') != string::npos)
		{
			return;
		}
	}
	
	file.close();
	cout << "ERR: " << enum_name << " not found" << endl;
	return;
}

int enum_map::enum_map_compare(const enum_map& cmpMap)
{
	for (auto it : kv_map)
	{
		if (cmpMap.kv_map.find(it.first) == cmpMap.kv_map.end())
		{
			cout << "WRN: {" << it.first << "} not found" << endl;
		}
		else if (it.second != cmpMap.kv_map.at(it.first))
		{
			cout << "ERR: {" << it.first << "} not equal: " << "{" << it.second << "}, {" << cmpMap.kv_map.at(it.first) << "}" << endl;
		}

		//cout << it.first << " " << it.second << ", " << cmpMap.kv_map.at(it.first) << endl;
	}

	return 0;
}

int main()
{
	string path1 = "test.c";
	string name1 = "test1";

	string path2 = "test.c";
	string name2 = "test2";

	enum_map eMap1(path1, name1);
	enum_map eMap2(path2, name2);
	eMap1.enum_map_compare(eMap2);
	eMap2.enum_map_compare(eMap1);
}