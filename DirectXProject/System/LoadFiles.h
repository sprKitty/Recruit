#pragma once
#include <string>
#include <map>
#include <vector>

using STRINGMAP = std::map<std::string, std::vector<std::string> >;

class LoadFiles
{
public:

public:
	LoadFiles() {}
	virtual ~LoadFiles() {}

	const STRINGMAP& CSV(const char* pPath);

private:
	STRINGMAP m_StringMap;
};