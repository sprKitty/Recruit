#include "LoadFiles.h"
#include <iostream>
#include <fstream>
#include <sstream>

const STRINGMAP& LoadFiles::CSV(const char * pPath)
{
	std::ifstream readfile;
	readfile.open(pPath, std::ios::in);

	if (!readfile.is_open())
	{
		std::cout << "CSVƒtƒ@ƒCƒ‹" << pPath << "‚ª“Ç‚Ýž‚ß‚Ü‚¹‚ñ‚Å‚µ‚½" << std::endl;
		return m_StringMap;
	}
	std::string line;
	while (std::getline(readfile, line))
	{
		std::istringstream i_stream(line);
		std::string title;
		std::string word;
		std::vector<std::string> texts;

		while (std::getline(i_stream, word, ','))
		{
			if (word.empty())continue;

			if (word[0] == '(')break;

			if (word[0] == '[')
			{
				word.erase(word.begin());
				title = word;
			}
			else
			{
				texts.push_back(word);
			}
		}
		if (title.empty())continue;
		if (texts.empty())continue;
		m_StringMap[title] = texts;
	}

	return m_StringMap;
}