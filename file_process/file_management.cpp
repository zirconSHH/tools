#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <locale>

#include "file_manegement.h"
#include "string_process.h"

using namespace std;
using namespace std::filesystem;

void show_locale()
{
	locale loc = locale::global(locale::classic());

	cout << "current locale: " << loc.name().c_str() << endl;
}

bool check_path(const path& targetPath) {
	if (!exists(targetPath)) {
		cout << "target path: " << absolute(targetPath) << " not found" << endl;
		return false;
	}

	cout << "target path: " << absolute(targetPath) << endl;

	return true;
}

bool need_check_duplicate(string targetPath) {
	size_t leftPos = targetPath.find_last_of('(');
	size_t rightPos = targetPath.find_last_of(')');

	if (!leftPos || !rightPos || leftPos >= rightPos)
		return false;

	if (leftPos + 1 <= targetPath.length()) {
		string checkDigit = targetPath.substr(leftPos + 1, rightPos - leftPos - 1);
		if (!checkDigit.empty() && all_of(checkDigit.begin(), checkDigit.end(), ::isdigit))
			return true;
	}

	return false;
}

string get_original_name(const path& targetPath) {
	if (!exists(targetPath))
		return "";

	string pathName = targetPath.filename().string();
	if (!need_check_duplicate(pathName)) {
		return pathName;
	}

	pathName.erase(pathName.find_last_of('('));
	pathName.erase(pathName.find_last_not_of(" ") + 1);

	return pathName;
}

bool is_subdirectory(const path& parent_dir, const path& subdir) {
	if (!exists(parent_dir) || !exists(subdir))
		return false;

	// path parent_abs_path = parent_dir.parent_path();
	path subdir_parent_path = subdir.parent_path();

	if (!exists(subdir_parent_path) || !is_directory(subdir_parent_path)) 
		return false;

	return starts_with(subdir_parent_path.string(), parent_dir.string());
	// return subdir_parent_path.string().starts_with(parent_abs_path.string());
	// return true;
}

bool manual_check = true;

template <class dir_info>
int directory_handle(const directory_entry& entry, map<string, dir_info>& paths) {
	const auto& path = entry.path();
	if (!exists(path))
		return -1;

	if (is_directory(path)) {
		string name = get_original_name(path);
		auto it = paths.find(name);
		if (it == paths.end()) {
			paths[name] = entry;
			return 0;
		}
		else {
			if (is_subdirectory(it->second.path(), path)) {
				cout << path << "same name parent path: " << it->second.path() << endl;
				return 0;
			}

			string rm;
			if (manual_check)
			{
				cout << path << "already exits: " << paths[name].path() << ", remove?" << endl;
				cin >> rm;
			}

			if (rm == "y" || !manual_check) {
				if (remove_all(path))
					cout << path << "removed" << endl;
				else
					cout << path << "remove failed" << endl;
			}
		}
	}

	return 0;
}

void remove_duplicate_directory(const path& path) {
	if (!exists(path))
		return;

	map<string, directory_entry> diretories;
	// map<string, directory_entry> removedDirs;

	for (const auto& entry : /* recursive_directory_iterator */directory_iterator(path)) {
		directory_handle(entry, diretories);
		// remove_duplicate_directory(path);
	}
}

int main() {
	show_locale();
	system("chcp 65001 > nul");
	cout << "current directroy: " << current_path() << endl;
	cout << "input target path: " << endl;

	string targetDirectory;
	getline(cin, targetDirectory);

	path targetPath(targetDirectory);
	if (!check_path(targetPath))
		return -1;

	remove_duplicate_directory(targetPath);

	cout << "done" << endl;

	return 0;
}