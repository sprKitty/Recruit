#include "MagicRazer.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer/BillBoardRenderer.h>
#include <App/Component/Transform.h>



void MagicRazer::Init()
{
	m_pTransform = m_pOwner.lock()->GetComponent<Transform>();
	m_vPos = 0;
}

void MagicRazer::Uninit()
{
}

void MagicRazer::Update()
{
	m_pTransform.lock()->localpos = m_vPos;
	m_pTransform.lock()->localscale.z = m_fLength;
	m_pTransform.lock()->localrot.y = -DirectX::XMConvertToDegrees(MyMath::Radian(0, 0, m_vDirection.x, m_vDirection.z)) + 90.0f;
}