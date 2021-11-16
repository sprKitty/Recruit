#include "TexAnimation.h"
#include <App/Component/Image.h>
#include <System/LoadFiles.h>
#include <System/Clocker.h>
#include <iostream>

void TexAnimation::LoadData(const char * pPath)
{
	LoadFiles lf;
	STRINGMAP strMap = lf.CSV(pPath);
	m_nAnimType = -1;
	m_nSheet = 0;
	m_nSheetsX = -1;
	m_nSheetsY = -1;
	m_fTime = 0.0f;
	m_fAnimSpeed = 0.0f;

	if (strMap.empty())return;

	m_pImage.reset(new Image());
	for (const auto& itr : strMap)
	{
		if (itr.first == "2アニメーションスピード")
		{
			m_fAnimSpeed = std::stof(itr.second[0]);
		}
		else if (itr.first == "3分割数")
		{
			m_pImage->m_vTiling.x = 1.0f / std::stoi(itr.second[0]);
			m_pImage->m_vTiling.y = 1.0f / std::stoi(itr.second[1]);
		}
		else if (itr.first == "5初期オフセット")
		{
			Vector2 vTile = m_pImage->m_vTiling;
			for (int i = 0; i < itr.second.size(); i+=2)
			{
				m_vStartOffsetList.push_back(Vector2(std::stof(itr.second[i]) * vTile.x, std::stof(itr.second[i + 1]) * vTile.y));
			}
		}
		else if (itr.first == "1ファイル名")
		{
			m_pImage->SetPath(itr.second[0].c_str());
		}
		else if (itr.first == "4アニメーション枚数")
		{
			m_nSheetsX = std::stoi(itr.second[0]);
			m_nSheetsY = std::stoi(itr.second[1]);
			if ((m_nSheetsX > 0) && (m_nSheetsY == 0))
			{
				m_nAnimType = 0;
			}
			else if ((m_nSheetsX == 0) && (m_nSheetsY > 0))
			{
				m_nAnimType = 1;
			}
			else if ((m_nSheetsX > 0) && (m_nSheetsY > 0))
			{
				m_nAnimType = 2;
			}
			else if ((m_nSheetsX == 0) && (m_nSheetsY == 0))
			{
				m_nAnimType = 3;
			}
		}
	}
}

void TexAnimation::Reset()
{

}

void TexAnimation::Update(const int nType)
{
	if (m_nAnimType < 0)return;

	switch (m_nAnimType)
	{
	case 0:
	{
		if (m_fTime < m_fAnimSpeed)
		{
			m_fTime += Clocker::GetInstance().GetFrameTime();
		}
		else
		{
			m_fTime -= m_fAnimSpeed;
			++m_nSheet;
			if (m_nSheet >= m_nSheetsX)
			{
				m_nSheet = 0;
			}
		}
		if (nType < m_vStartOffsetList.size())
		{
			m_pImage->m_vOffset.x = m_vStartOffsetList[nType].x + m_pImage->m_vTiling.x * m_nSheet;
			m_pImage->m_vOffset.y = m_vStartOffsetList[nType].y;
		}
		else
		{
			std::cout << "TexAnimationクラスのUpdate case0で配列外参照が起こっています" << std::endl;
		}
		return;
	}
	case 1:
		return;

	case 2:
		return;

	case 3:
	{
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
