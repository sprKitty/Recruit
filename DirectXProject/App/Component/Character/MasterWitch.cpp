#include "MasterWitch.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <System/Clocker.h>
#include <System/DebugLog.h>
#include <MyMath.h>

const char* g_pWitchAnimPath[Witch_State::MAX] =
{
	"Assets/csv/witchwait.csv",
};

void MasterWitch::Init()
{
	m_Direction = Chara_Direction::DOWN;
	for (int i = 0; i < Witch_State::MAX; ++i)
	{
		std::shared_ptr<TexAnimation> pImage(new TexAnimation());
		pImage->LoadData(g_pWitchAnimPath[i]);
		m_pTexAnimList.push_back(pImage);
	}
}

void MasterWitch::Uninit()
{
}

void MasterWitch::Update()
{
	m_Transform = m_pOwner.lock()->GetTransform();

	CalcTarget();
	m_pTexAnimList[Witch_State::WAIT]->Update(m_Direction);
	if (!m_pBBR.expired())
	{
		m_pBBR.lock()->SetMainImage(m_pTexAnimList[Witch_State::WAIT]);
	}
	m_pOwner.lock()->SetTransform(m_Transform);
}

void MasterWitch::CalcTarget()
{
	if (m_pTarget.expired())
	{
		DebugLog::GetInstance().FreeError("MasterWitchクラスでターゲットが設定されていません。");
		return;
	}
	Transform targetT = m_pTarget.lock()->GetTransform();

	Vector2 vDistance = Vector2(m_Transform.pos.x - targetT.pos.x, m_Transform.pos.z - targetT.pos.z);
	float fLength = vDistance.Length();

	if (fLength > 5)
	{
		m_Direction = Chara_Direction::DOWN;
	}
	else
	{
		float fRad = MyMath::Radian(m_Transform.pos.x, m_Transform.pos.z, targetT.pos.x, targetT.pos.z);

		m_Direction = CalcDirection(DEG(fRad));
	}
}
