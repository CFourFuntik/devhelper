#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

std::vector<std::string> split(const std::string &s, char delim);
namespace LOG
{
	void write(const std::string &s);
}