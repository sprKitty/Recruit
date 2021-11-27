#include "TexAnimation.h"
#include <App/Component/Image.h>
#include <System/LoadFiles.h>
#include <System/Clocker.h>
#include <System/DebugLog.h>
#include <iostream>

void TexAnimation::LoadData(const char * pPath)
{
	LoadFiles lf;
	STRINGMAP strMap = lf.CSV(pPath);
	m_nAnimType = -1;
	m_nNowAnim = 0;
	m_vSheet = 0;
	m_vSheetsNum = -1;
	m_vStartSheet = -1;
	m_fTime = 0.0f;
	m_fAnimSpeed = 0.0f;
	m_isLoop = true;
	m_isFinish = m_isSheetUpdate = false;

	if (strMap.empty())return;

	m_pImage.reset(new Image());
	for (const auto& itr : strMap)
	{
		if (itr.first == "1ファイル名")
		{
			std::string path("Assets/characterchip/");
			path += itr.second[0];
			m_pImage->SetPath(path.c_str());
		}
		else if (itr.first == "2アニメーションスピード")
		{
			m_fAnimSpeed = std::stof(itr.second[0]);
		}
		else if (itr.first == "3ループ処理")
		{
			if (itr.second[0] == "する")
			{
				m_isLoop = true;
			}
			else if(itr.second[0] == "しない")
			{
				m_isLoop = false;
			}
		}
		else if (itr.first == "4分割数")
		{
			m_pImage->m_vTiling.x = 1.0f / std::stoi(itr.second[0]);
			m_pImage->m_vTiling.y = 1.0f / std::stoi(itr.second[1]);
		}
		else if (itr.first == "5アニメーション枚数")
		{
			m_vSheetsNum.x = std::stoi(itr.second[0]);
			m_vSheetsNum.y = std::stoi(itr.second[1]);
			if ((m_vSheetsNum.x > 0) && (m_vSheetsNum.y == 0))
			{
				m_nAnimType = 0;
			}
			else if ((m_vSheetsNum.x == 0) && (m_vSheetsNum.y > 0))
			{
				m_nAnimType = 1;
			}
			else if ((m_vSheetsNum.x > 0) && (m_vSheetsNum.y > 0))
			{
				m_nAnimType = 2;
			}
			else if ((m_vSheetsNum.x == 0) && (m_vSheetsNum.y == 0))
			{
				m_nAnimType = 3;
			}
		}
		else if (itr.first == "6初期オフセット")
		{
			Vector2 vTile = m_pImage->m_vTiling;
			for (int i = 0; i < itr.second.size(); i+=2)
			{
				m_vStartOffsetList.push_back(Vector2(std::stof(itr.second[i]) * vTile.x, std::stof(itr.second[i + 1]) * vTile.y));
			}
		}
		else if (itr.first == "7スタートシート")
		{
			m_vStartSheet.x = std::stoi(itr.second[0]);
			m_vStartSheet.y = std::stoi(itr.second[1]);
			m_vSheet = m_vStartSheet;
		}
	}
}

void TexAnimation::Reset(const int nType)
{
	m_vSheet = m_vStartSheet;
	m_nNowAnim = 0;
	switch (m_nAnimType)
	{
	case 0:
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_vSheet.x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y;
		break;

	case 1:
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y + m_pImage->m_vTiling.y * m_vSheet.y;
		break;

	case 2:
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_vSheet.x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y + m_pImage->m_vTiling.y * m_vSheet.y;
		break;

	case 3:
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_vSheet.x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y + m_pImage->m_vTiling.y * m_vSheet.y;
		break;
	default:
		break;
	}
}

void TexAnimation::Update(const int nType)
{
	if (m_nAnimType < 0)return;
	if (nType >= m_vStartOffsetList.size())
	{
		DebugLog::GetInstance().OutofRange("TexAnimationクラスのUpdate");
		return;
	}
	if (!m_isLoop)
	{
		m_isFinish = false;
	}
	m_isSheetUpdate = false;
	switch (m_nAnimType)
	{
	case 0:
	{
		if (m_fTime < m_fAnimSpeed)
		{
			m_fTime += static_cast<float>(Clocker::GetInstance().GetFrameTime());
		}
		else
		{
			m_fTime = 0;
			m_isSheetUpdate = true;
			++m_vSheet.x;
			++m_nNowAnim;
			if (m_vSheet.x >= m_vSheetsNum.x)
			{
				m_vSheet.x = 0;
			}
			if (m_nNowAnim >= m_vSheetsNum.x)
			{
				m_nNowAnim = 0;
				if (!m_isLoop)
				{
					m_isFinish = true;
				}
			}
		}
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_vSheet.x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y;
		return;
	}
	case 1:
	{
		if (m_fTime < m_fAnimSpeed)
		{
			m_fTime += static_cast<float>(Clocker::GetInstance().GetFrameTime());
		}
		else
		{
			m_fTime = 0;
			m_isSheetUpdate = true;
			++m_vSheet.y;
			++m_nNowAnim;
			if (m_vSheet.y >= m_vSheetsNum.y)
			{
				m_vSheet.y = 0;
			}
			if (m_nNowAnim >= m_vSheetsNum.y)
			{
				m_nNowAnim = 0;
				if (!m_isLoop)
				{
					m_isFinish = true;
				}
			}
		}
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y + m_pImage->m_vTiling.y * m_vSheet.y;
		return;
	}

	case 2:
	{
		return;
	}

	case 3:
	{
		m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_vSheet.x;
		m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y + m_pImage->m_vTiling.y * m_vSheet.y;
		return;
	}
	default:
		return;
	}
}

void TexAnimation::Bind()
{
	m_pImage->Bind();
}

const bool TexAnimation::IsSheetUpdate(const VectorInt2& vSheet)
{
	switch (m_nAnimType)
	{
	case 0:
		if (m_nNowAnim == vSheet.x)
		{
			if (m_isSheetUpdate)return true;
		}
		break;
	case 1:
		if (m_nNowAnim == vSheet.y)
		{
			if (m_isSheetUpdate)return true;
		}
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	return false;
}
