#pragma once

#include <iostream>
#include <filesystem>

using namespace std::filesystem;

class directory_information
{
private:
	directory_entry entry;
public:
	directory_information(const directory_entry& dirEntry): entry(dirEntry)
	{}
};

