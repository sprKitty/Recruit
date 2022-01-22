#include "ViewPoint.h"

ViewPoint::ViewPoint()
{
	targetTransform = Property<std::weak_ptr<Transform> >(&m_pTargetTransform);
	view = Property<DirectX::XMMATRIX>(&m_mView);
	projection = Property<DirectX::XMMATRIX>(&m_mProjection);
	worldMatrix = Property<DirectX::XMFLOAT4X4>(&m_mWorldMatrix);
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
	m_frustum.resize(FRUSTUM_SIZE);
	m_frustumWorld.resize(FRUSTUM_SIZE);
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

void ViewPoint::CalcReflect(const Vector3* pPoint, const Vector3* pNormal)
{
	Vector3 vNormal(0.f, 1.f, 0.f);
	Vector3 vPoint(0.f, 0.f, 0.f);
	if (pNormal) vNormal = *pNormal;
	if (pPoint) vPoint = *pPoint;

	Vector3 vA = m_vPos + m_vFront * m_fFarClip;
	Vector3 vB = m_vPos;
	Vector3 vPA = vA - vPoint;
	Vector3 vPB = vB - vPoint;
	float dot_PA = vPA.Dot(vNormal);
	float dot_PB = vPB.Dot(vNormal);

	//線端が平面上にあった時の計算の誤差を吸収。
	if (abs(dot_PA) < 0.001f) dot_PA = 0.0f;
	if (abs(dot_PB) < 0.001f) dot_PB = 0.0f;

	//交差判定
	if (dot_PA == 0.0f && dot_PB == 0.0f)
	{
		//両端が平面上にあり、交点を計算できない。
		return;
	}
	else if ((dot_PA >= 0.0f && dot_PB >= 0.0f)
		|| (dot_PA <= 0.0f && dot_PB <= 0.0f))
	{
		//交差していない
		return;
	}

	m_vKeepLook = m_vLook;
	m_vKeepPos = m_vPos;
	Vector3 vBA = vB - vA;

	float fRatio = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

	m_vLook = vA + (vBA * fRatio);

	Vector3 vDis = m_vLook - m_vPos;
	vDis *= -1;
	float a = vDis.Dot(vNormal);
	vDis *= -1;
	m_vPos = vDis + vNormal * a * 2.0f;
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
}

const FrustumType::kind ViewPoint::CollisionViewFrustum(const DirectX::XMFLOAT3& pos, const float fRadius)
{
	bool isHit = false;
	DirectX::XMVECTOR vFrusW, vCenter, vDot;
	float fDot;
	vCenter = DirectX::XMLoadFloat3(&pos);

	for (auto& itr : m_frustumWorld)
	{
		vFrusW = DirectX::XMLoadFloat4(&itr);
		vDot = DirectX::XMPlaneDotCoord(vFrusW, vCenter);
		DirectX::XMStoreFloat(&fDot, vDot);
		if (fDot < -fRadius)
			return FrustumType::OUTSIDE;	// 完全に外側
		if (fDot <= fRadius)
			isHit = true;
	}
	if (isHit)
		return FrustumType::PARTLYINSIDE;	// 部分的に内側
	return FrustumType::INSIDE;	// 完全に内側
}

ID3D11ShaderResourceView * ViewPoint::GetRenderingTexture(int num)
{
	if (!m_pRenderTarget.get())nullptr;
	if (num > m_pRenderTarget->GetNum())nullptr;
	
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

void ViewPoint::CalcWorldMatrix()
{
	m_mWorldMatrix._11 = m_vSide.x;
	m_mWorldMatrix._12 = m_vSide.y;
	m_mWorldMatrix._13 = m_vSide.z;
	m_mWorldMatrix._14 = 0.0f;
	m_mWorldMatrix._21 = m_vUp.x;
	m_mWorldMatrix._22 = m_vUp.y;
	m_mWorldMatrix._23 = m_vUp.z;
	m_mWorldMatrix._24 = 0.0f;
	m_mWorldMatrix._31 = m_vFront.x;
	m_mWorldMatrix._32 = m_vFront.y;
	m_mWorldMatrix._33 = m_vFront.z;
	m_mWorldMatrix._34 = 0.0f;
	m_mWorldMatrix._41 = m_vPos.x;
	m_mWorldMatrix._42 = m_vPos.y;
	m_mWorldMatrix._43 = m_vPos.z;
	m_mWorldMatrix._44 = 1.0f;
}

void ViewPoint::CreateViewFrustum()
{
	float fTan = tanf(DirectX::XMConvertToRadians(m_fFov) * 0.5f);
	m_frustum[0] = DirectX::XMFLOAT4(0.0f, -1.0f, fTan, 0.0f);
	m_frustum[1] = DirectX::XMFLOAT4(0.0f, 1.0f, fTan, 0.0f);
	fTan *= GetAspect();
	m_frustum[2] = DirectX::XMFLOAT4(1.0f, 0.0f, fTan, 0.0f);
	m_frustum[3] = DirectX::XMFLOAT4(-1.0f, 0.0f, fTan, 0.0f);
	m_frustum[4] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, -m_fNearClip);
	m_frustum[5] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, m_fFarClip);
	
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMStoreFloat4(&m_frustum[i], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_frustum[i])));
	}
}

void ViewPoint::UpdateViewFrustum()
{
	DirectX::XMMATRIX mtx = DirectX::XMLoadFloat4x4(&m_mWorldMatrix);
	mtx = DirectX::XMMatrixInverse(nullptr, mtx);
	mtx = DirectX::XMMatrixTranspose(mtx);
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMStoreFloat4(&m_frustumWorld[i], DirectX::XMPlaneTransform(DirectX::XMLoadFloat4(&m_frustum[i]), mtx));
	}
}
