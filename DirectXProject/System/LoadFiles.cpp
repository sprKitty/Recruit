#include "LoadFiles.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <System/DebugLog.h>

const STRINGMAP& LoadFiles::CSV(const char * pPath)
{
	std::ifstream readfile;
	readfile.open(pPath, std::ios::in);

	if (!readfile.is_open())
	{
		DebugLog::GetInstance().LoadFileError(pPath);
		//std::cout << "CSVファイル" << pPath << "が読み込めませんでした" << std::endl;
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
				texts.emplace_back(word);
			}
		}
		if (title.empty())continue;
		if (texts.empty())continue;
		m_StringMap[title] = texts;
	}

	return m_StringMap;
}