#include "RootMotion.h"

void RootMotion::Init()
{
}

void RootMotion::Uninit()
{
}

void RootMotion::Update()
{
}

void RootMotion::CalcRoot(const Vector3& vDestination, const Vector3 vNowPos, bool isReset)
{
	if (isReset)m_calcPointInfoList = m_pointInfoList;	// 計算をリセット
	std::vector<Vector3> vRootA, vRootB;
	std::vector<int> arrayList;

	m_vRootList.clear();
	// 
	MinPointArray(&m_calcPointInfoList, vNowPos);

	if (m_calcPointInfoList.size() < 2)	// ルート計算ができなくなるので強制リセット
	{
		m_calcPointInfoList = m_pointInfoList;
		MinPointArray(&m_calcPointInfoList, vNowPos);
	}

	float lengthA = Vector3(vDestination - m_calcPointInfoList[0].vPoint).Length();
	float lengthB = Vector3(vDestination - m_calcPointInfoList[1].vPoint).Length();

	int num = 0;
	if (m_calcPointInfoList[0].vNext.x >= 0)
	{
		if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint).Length() <= lengthA)
		{
			num = 1;
			lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint).Length();
		}
	}

	if (m_calcPointInfoList[0].vNext.y >= 0)
	{
		if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint).Length() <= lengthA)
		{
			num = 2;
			lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint).Length();
		}
	}

	if (m_calcPointInfoList[0].vNext.z >= 0)
	{
		if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint).Length() <= lengthA)
		{
			num = 3;
			lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint).Length();
		}
	}

	if (m_calcPointInfoList[0].vNext.w >= 0)
	{
		if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint).Length() <= lengthA)
		{
			num = 4;
			lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint).Length();
		}
	}

	//switch (num)
	//{
	//case 1:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint);
	//	break;

	//case 2:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint);
	//	break;

	//case 3:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint);
	//	break;

	//case 4:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint); 
	//	break;

	//case 0:
	//default:
	//	break;
	//}

	vRootA.emplace_back(m_calcPointInfoList[0].vPoint);
	vRootA.emplace_back(vDestination);


	//num = 0;
	//if (m_calcPointInfoList[0].vNext.x >= 0)
	//{
	//	if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint).Length() <= lengthA)
	//	{
	//		num = 1;
	//		lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint).Length();
	//	}
	//}

	//if (m_calcPointInfoList[0].vNext.y >= 0)
	//{
	//	if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint).Length() <= lengthA)
	//	{
	//		num = 2;
	//		lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint).Length();
	//	}
	//}

	//if (m_calcPointInfoList[0].vNext.z >= 0)
	//{
	//	if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint).Length() <= lengthA)
	//	{
	//		num = 3;
	//		lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint).Length();
	//	}
	//}

	//if (m_calcPointInfoList[0].vNext.w >= 0)
	//{
	//	if (Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint).Length() <= lengthA)
	//	{
	//		num = 4;
	//		lengthA = Vector3(m_calcPointInfoList[0].vPoint - m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint).Length();
	//	}
	//}

	//switch (num)
	//{
	//case 1:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.x].vPoint);
	//	break;

	//case 2:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.y].vPoint);
	//	break;

	//case 3:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.z].vPoint);
	//	break;

	//case 4:
	//	vRootA.emplace_back(m_calcPointInfoList[m_calcPointInfoList[0].vNext.w].vPoint);
	//	break;

	//case 0:
	//default:
	//	break;
	//}

	vRootB.emplace_back(m_calcPointInfoList[1].vPoint);
	vRootB.emplace_back(vDestination);

	lengthA = Vector3(vNowPos - vRootA[0]).Length();
	for (int i = 1; i < vRootA.size(); ++i)
	{
		lengthA += Vector3(vRootA[i - 1] - vRootA[i]).Length();
	}

	lengthB = Vector3(vNowPos - vRootB[0]).Length();
	for (int i = 1; i < vRootB.size(); ++i)
	{
		lengthB += Vector3(vRootB[i - 1] - vRootB[i]).Length();
	}

	int ans = (lengthA < lengthB) ? 0 : 1;
	m_calcPointInfoList[ans].isUsed = true;
	m_vRootList = (lengthA < lengthB) ? vRootA : vRootB;	// ルートAとBの距離を比較し近いほうを採用 
}

void RootMotion::AddPoint(const Vector3 & vPoint, const VectorInt4 & vNext)
{
	Info info;
	info.vPoint = vPoint;
	info.vNext = vNext;
	info.isUsed = false;
	m_pointInfoList.emplace_back(info);
}

const Vector3& RootMotion::CalcGuide(const Vector3 & vNowPos)
{
	if (m_vRootList.empty())return vNowPos;

	float fLength = Vector2(vNowPos.x - m_vRootList.begin()->x, vNowPos.z - m_vRootList.begin()->z).Length();
	if (fLength <= 0.2f) // 誤差が許容範囲以内なら次の目的地へ
	{
		m_vRootList.erase(m_vRootList.begin());
	}
	
	if (m_vRootList.empty())return vNowPos;
	return *m_vRootList.begin();
}

void RootMotion::MinPointArray(std::vector<Info>* pPointList, const Vector3& vNowPos)
{
	std::vector<Info> vPointList = *pPointList;
	pPointList->clear();

	while (!vPointList.empty())
	{
		float fLength = FLT_MAX;
		std::vector<Info>::iterator minItr = vPointList.end();
		for (auto itr = vPointList.begin(); itr < vPointList.end(); ++itr)
		{
			if (itr->isUsed)continue;
			float fL = Vector3(vNowPos - itr->vPoint).Length();
			if (fL < fLength)
			{
				fLength = fL;
				minItr = itr;
			}
		}
		if (minItr == vPointList.end())
		{
			minItr = vPointList.begin();
		}
		pPointList->emplace_back(*minItr);
		vPointList.erase(minItr);
	}
}
