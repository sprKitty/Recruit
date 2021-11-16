#include "LoadFiles.h"
#include <iostream>
#include <fstream>
#include <sstream>

const STRINGMAP& LoadFiles::CSV(const char * pPath)
{
	std::ifstream readfile;
	readfile.open(pPath, std::ios::in);

	if (!readfile.is_open())return m_StringMap;

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

			if (word[0] == '(')continue;

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
		if (word.empty())continue;
		m_StringMap[title] = texts;
	}

	return m_StringMap;
}