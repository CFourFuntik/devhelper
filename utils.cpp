#include "utils.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

namespace LOG
{
	void write(const std::string &s)
	{
		char *pValue;
		size_t len;
		errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
		std::string fileName;
		if (err)
			fileName = "C:\\debug.txt";
		else
			fileName = std::string(pValue) + "\\Documents\\Rockstar Games\\GTA V\\debug.txt";
		delete[] pValue;
		std::ofstream log(fileName, std::fstream::app);
		log << s << std::endl;
		log.close();
	}
}