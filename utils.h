#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

#define CONFIG_FILE_NAME "devhelp.ini"

std::vector<std::string> split(const std::string &s, char delim);
void sendPlayerMessage(const std::string &message, int r = 255, int g = 255, int b = 255);

namespace LOG
{
	void write(const std::string &s);
}

namespace CONFIG
{
	bool get(const std::string &param, std::string &value);
	bool loadList(const std::string &filename, std::vector<std::string> &value);
}