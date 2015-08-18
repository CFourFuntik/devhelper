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

namespace CONFIG
{
	bool get(const std::string &param, std::string &value)
	{
		std::string line;
		std::ifstream config(CONFIG_FILE_NAME);
		if (!config.good()) return false;
		while (std::getline(config, line))
		{
			std::vector<std::string> temp;
			temp = split(line, ' ');
			if (!temp[0].compare(param))
			{
				value = temp[1];
				for (int i = 2; i < temp.size(); ++i)
				{
					value += std::string(" ") + temp[i];
				}
				config.close();
				return true;
			}
		}
		config.close();
		return false;
	}

	bool loadList(const std::string &filename, std::vector<std::string> &value)
	{
		std::string line;
		std::ifstream list(filename);
		if (!list.good()) return false;
		while (std::getline(list, line))
			value.push_back(line);
		list.close();
		return true;
	}
}