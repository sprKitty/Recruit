#pragma once
#include <System/ClassDesign/Singleton.h>
#include <string>
#include <fstream>

class DebugLog : public Singleton<DebugLog>
{
public:
	friend class Singleton<DebugLog>;

public:
	void Initialize()override;
	void Finalize()override;

	void LoadFileError(const char* pName);
	void OutofRange(const std::string string);
	void FreeError(const std::string string);

protected:
	DebugLog() {}
	~DebugLog()override {}

private:
	std::ofstream m_Write_File;
};