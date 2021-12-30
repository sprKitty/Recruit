#include "ViewPoint.h"

ViewPoint::ViewPoint()
{
	targetTransform = Property<std::weak_ptr<Transform> >(&m_pTargetTransform);
	view = Property<DirectX::XMMATRIX>(&m_mView);
	projection = Property<DirectX::XMMATRIX>(&m_mProjection);
	color = Property<Vector4>(&m_vColor);
	position = Property<Vector3>(&m_vPos);
	look = Property<Vector3>(&m_vLook);
	up = Property<Vector3>(&m_vUp);
	side = Property<Vector3>(&m_vSide);
	front = Property<Vector3>(&m_vFront);
	vpSize = Property<Vector2>(&m_vScreenSize);
	parallelScale = Property<Vector2>(&m_vParallelScale);
	nearclip = Property<float>(&m_fNearClip);
	farclip = Property<float>(&m_fFarClip);
	fov = Property<float>(&m_fFov);
	perspective = Property<bool>(&m_isPerspective);
}

void ViewPoint::CopyParameter(const std::weak_ptr<ViewPoint> pVP)
{
	if (pVP.expired())return;

	m_mView = pVP.lock()->view.get();
	m_mProjection = pVP.lock()->projection.get();
	m_vColor = pVP.lock()->color.get();
	m_vPos = pVP.lock()->position.get();
	m_vLook = pVP.lock()->look.get();
	m_vUp = pVP.lock()->up.get();
	m_vSide = pVP.lock()->side.get();
	m_vFront = pVP.lock()->front.get();
	m_vParallelScale = pVP.lock()->parallelScale.get();
	m_fNearClip = pVP.lock()->nearclip.get();
	m_fFarClip = pVP.lock()->farclip.get();
	m_fFov = pVP.lock()->fov.get();
	m_isPerspective = pVP.lock()->perspective.get();
}

void ViewPoint::CalcRefletion(const Vector3* pLook, const Vector3* pNormal)
{
	Vector3 normal(0, 1, 0);
	m_vKeepLook = m_vLook;
	m_vKeepPos = m_vPos;
	Vector3 pos = m_vPos;
	Vector3 look = m_vLook;
	
	if (pLook)look = *pLook; // ’Ž‹“_‚ª•Ê‚ÅÝ’è‚³‚ê‚Ä‚¢‚½‚çã‘‚«
	if (pNormal)normal = *pNormal; // –@ü‚ª•Ê‚ÅÝ’è‚³‚ê‚Ä‚¢‚½‚çã‘‚«

	Vector3 dis = look - pos;
	dis *= -1;
	float a = dis.Dot(normal);
	dis *= -1;
	m_vLook = look;
	m_vPos = dis + normal * a * 2;
	m_vPos = m_vLook - m_vPos;
	m_mKeepView = m_mView;
	CalcView(1.0f);
}

void ViewPoint::Restore()
{
	m_vLook = m_vKeepLook;
	m_vPos = m_vKeepPos;
	m_mView = m_mKeepView;
}


void ViewPoint::BindRenderTarget()
{
	if (m_pRenderTarget.get())
	{
		DirectX11::GetInstance().BeginDraw(&m_pRenderTarget->GetViewPort(), m_pRenderTarget->GetRTV(), m_pRenderTarget->GetDepthStecil(), &m_pRenderTarget->GetClearColorList(), m_pRenderTarget->GetNum());
	}
	else
	{
		DirectX11::GetInstance().BeginDraw();
	}
}

ID3D11ShaderResourceView * ViewPoint::GetRenderingTexture(int num)
{
	if (!m_pRenderTarget.get())nullptr;
	if (num >= m_pRenderTarget->GetNum())nullptr;
	
	return m_pRenderTarget->GetSRV()[num];
}


void ViewPoint::CalcView(const float fUp)
{
	m_mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(m_vPos.x, m_vPos.y, m_vPos.z, 0),
		DirectX::XMVectorSet(m_vLook.x, m_vLook.y, m_vLook.z, 0),
		DirectX::XMVectorSet(0, fUp, 0, 0)
	);
}

void ViewPoint::CalcProjection()
{
	if (m_isPerspective)
	{
		m_mProjection = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(m_fFov), m_vScreenSize.x / m_vScreenSize.y, m_fNearClip, m_fFarClip
		);
	}
	else
	{
		Vector2 halfSize = m_vScreenSize / 2 / m_vParallelScale;
		m_mProjection = DirectX::XMMatrixOrthographicOffCenterLH(
			-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, m_fNearClip, m_fFarClip
		);
	}
}
