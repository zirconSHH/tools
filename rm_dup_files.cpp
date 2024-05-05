#include <bits/stdc++.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>

using namespace std;

class pathInfo
{
public:

	bool operator==(const pathInfo& other)
	{
		if (this->filePath != other.filePath)
			return false;

		// if (this->fileInfo != other.fileInfo)
		// 	return false;

		return true;
	}

	string filePath;
	__finddata64_t fileInfo;
};

int TraverseDirectory(string path, vector<pathInfo>& dirs, vector<pathInfo>& files)
{
	long long Handle;
	struct __finddata64_t  FileInfo;
	string strFind = path + "\\*";
 
	if ((Handle = _findfirst64(strFind.c_str(), &FileInfo)) == -1L)
	{
		cout << __func__ << " no valid file" << endl;
		return -1;
	}
	do
	{
		// check sub dirs
		if (FileInfo.attrib & _A_SUBDIR)
		{
			// except . & ..
			if ((strcmp(FileInfo.name, ".") == 0) || (strcmp(FileInfo.name, "..") == 0))
				continue;

			string newPath = path + "\\" + FileInfo.name;
			pathInfo tmpPathInfo = {newPath, FileInfo};
			if (find(dirs.begin(), dirs.end(), tmpPathInfo) == dirs.end())
			{
				dirs.emplace_back(tmpPathInfo);
			}
			TraverseDirectory(newPath, dirs, files);
		}
		//check files
		else
		{
			string newPath = path + "\\" + FileInfo.name;
			pathInfo tmpPathInfo = {newPath, FileInfo};
			if (find(dirs.begin(), dirs.end(), tmpPathInfo) == dirs.end())
			{
				files.emplace_back(tmpPathInfo);
			}
			
		}
	} while (_findnext64(Handle, &FileInfo) == 0);
 
	_findclose(Handle);
	return 0;
}

bool need_check_duplicate(string filePath)
{
	size_t leftPos = filePath.find_last_of('(');
	size_t rightPos = filePath.find_last_of(')');

	if (!leftPos || !rightPos || leftPos >= rightPos)
	{
		return false;
	}

	if (leftPos + 1 <= filePath.length())
	{
		string checkDigit = filePath.substr(leftPos + 1, rightPos - leftPos - 1);
		if (!checkDigit.empty() && all_of(checkDigit.begin(), checkDigit.end(), ::isdigit))
		{
			return true;
		}
	}

	return false;
}

string get_original_file_path(string filePath)
{
	if (!need_check_duplicate(filePath))
		return "";

	size_t leftPos = filePath.find_last_of('(');
	string rst = filePath.substr(0, leftPos);
	if (rst[rst.length()] == ' ')
		rst = rst.substr(0, rst.length() - 1);

	return rst;
}

bool original_file_exist(vector<pathInfo>& paths, string filePath)
{
	string original = get_original_file_path(filePath);
	if (original == "")
	{
		cout << __func__ << " get original file error" << endl;
		return false;
	}

	bool found = false;
	for (auto& it: paths)
	{
		if (it.filePath == filePath)
			found = true;
	}
	if (!found)
		return false;

	return true;
}

int remove_dir_windows(string filePath)
{
	string delCmd = "rmdir /s /q " + string(1,'\"') + filePath + '\"';
	char *del=(char*)delCmd.c_str();
	cout << del << endl;
	system(del);

	return 0;
}

int remove_duplicate_files_directories(vector<pathInfo>& path, vector<pathInfo>& already_removed)
{
	for (auto& it_path: path)
	{
		if (!need_check_duplicate(it_path.filePath))
			continue;
		
		//check if current file is already removed
		bool isRemoved = false;
		for (auto& it_removed: already_removed)
		{
			if (it_path.filePath == it_removed.filePath || it_path.filePath.find(it_removed.filePath) != string::npos)
			{
				cout << it_path.filePath << " already removed" << endl;
				isRemoved = true;
				break;
			}
		}
		// check if original file exists
		if (!isRemoved)
		{
			if (original_file_exist(path, it_path.filePath))
			{
				//remove file
				remove_dir_windows(it_path.filePath);
				cout << "removed" << it_path.filePath << ", found original file" << get_original_file_path(it_path.filePath) << endl;
				already_removed.emplace_back(it_path);
			}
			else
			{
				cout << "found" << it_path.filePath << "without original file" << get_original_file_path(it_path.filePath) << endl;
			}
		}
	}

	return 0;
}

int main()
{
	string targetPath = "..";
	vector<pathInfo> dirs;
	vector<pathInfo> files;
	TraverseDirectory(targetPath, dirs, files);
	vector<pathInfo> already_removed;
	remove_duplicate_files_directories(dirs, already_removed);
	// remove_duplicate_files_directories(files, already_removed);

	return 0;
}
