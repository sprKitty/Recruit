#include "DebugLog.h"
#include <System/Clocker.h>
#include <Windows.h>
#include <iostream>

void DebugLog::Initialize()
{
#ifdef _DEBUG
	// コンソール作成
	AttachConsole(ATTACH_PARENT_PROCESS);
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	SetConsoleTitle("Debug Console");
	SMALL_RECT rect = { 0,0,50,25 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(handle, true, &rect);

	std::string fileName = "Assets/log/errorlog.txt";
	m_Write_File.open(fileName, std::ios::out);
#endif // _DEBUG
}

void DebugLog::Finalize()
{
#ifdef _DEBUG
	m_Write_File.close();

	FreeConsole();
#endif // _DEBUG
}

void DebugLog::LoadFileError(const char * pName)
{
#ifdef _DEBUG
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += pName;
	str += "が読み込めませんでした。";
	m_Write_File << str << std::endl;
	std::cout << str << std::endl;
#endif // _DEBUG
}

void DebugLog::OutofRange(const std::string string)
{
#ifdef _DEBUG
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += string;
	str += "で配列外参照が起こりました。";
	m_Write_File << str << std::endl;
	std::cout << str << std::endl;
#endif // _DEBUG
}

void DebugLog::FreeError(const std::string string)
{
#ifdef _DEBUG
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += string;
	m_Write_File << str << std::endl;
	std::cout << str << std::endl;
#endif // _DEBUG
}
