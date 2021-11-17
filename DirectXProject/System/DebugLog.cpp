#include "DebugLog.h"
#include <System/Clocker.h>
#include <Windows.h>
#include <iostream>

void DebugLog::Initialize()
{
#ifdef _DEBUG
	// �R���\�[���쐬
	AttachConsole(ATTACH_PARENT_PROCESS);
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	SetConsoleTitle("Debug Console");
#endif // _DEBUG

	std::string fileName = "Assets/log/errorlog.txt";
	m_Write_File.open(fileName, std::ios::out);
}

void DebugLog::Finalize()
{
	m_Write_File.close();

#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG
}

void DebugLog::LoadFileError(const char * pName)
{
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += pName;
	str += "���ǂݍ��߂܂���ł����B";
	m_Write_File << str << std::endl;
#ifdef _DEBUG
	std::cout << str << std::endl;
#endif // _DEBUG
}

void DebugLog::OutofRange(const std::string string)
{
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += string;
	str += "�Ŕz��O�Q�Ƃ��N����܂����B";
	m_Write_File << str << std::endl;
#ifdef _DEBUG
	std::cout << str << std::endl;
#endif // _DEBUG
}

void DebugLog::FreeError(const std::string string)
{
	std::string str(Clocker::GetInstance().GetDateTimeStr());
	str += " ";
	str += string;
	m_Write_File << str << std::endl;
#ifdef _DEBUG
	std::cout << str << std::endl;
#endif // _DEBUG
}
