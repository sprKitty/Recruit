#include "ViewPoint.h"

ViewPoint::ViewPoint()
{
	//targetTransform = Property<std::weak_ptr<Transform> >(&m_pTargetTransform);
	//view = Property<DirectX::XMMATRIX>(&m_mView);
	//projection = Property<DirectX::XMMATRIX>(&m_mProjection);
	//worldMatrix = Property<DirectX::XMFLOAT4X4>(&m_mWorldMatrix);
	//color = Property<Vector4>(&m_vColor);
	//position = Property<Vector3>(&m_vPos);
	//look = Property<Vector3>(&m_vLook);
	//up = Property<Vector3>(&m_vUp);
	//side = Property<Vector3>(&m_vSide);
	//front = Property<Vector3>(&m_vFront);
	//vpSize = Property<Vector2>(&m_vScreenSize);
	//parallelScale = Property<Vector2>(&m_vParallelScale);
	//nearclip = Property<float>(&m_fNearClip);
	//farclip = Property<float>(&m_fFarClip);
	//fov = Property<float>(&m_fFov);
	//perspective = Property<bool>(&m_isPerspective);
	//m_frustum.resize(FRUSTUM_SIZE);
	//m_frustumWorld.resize(FRUSTUM_SIZE);
}

void ViewPoint::CopyParameter(const std::weak_ptr<ViewPoint> pVP)
{
	if (pVP.expired())return;

	view.set(pVP.lock()->view.get());
	projection.set(pVP.lock()->projection.get());
	color.set(pVP.lock()->color.get());
	position.set(pVP.lock()->position.get());
	look.set(pVP.lock()->look.get());
	up.set(pVP.lock()->up.get());
	side.set(pVP.lock()->side.get());
	front.set(pVP.lock()->front.get());
	vpSize.set(pVP.lock()->vpSize.get());
	parallelScale.set(pVP.lock()->parallelScale.get());
	nearclip.set(pVP.lock()->nearclip.get());
	farclip.set(pVP.lock()->farclip.get());
	fov.set(pVP.lock()->fov.get());
	perspective.set(pVP.lock()->perspective.get());

	//m_mView = pVP.lock()->view.get();
	//m_mProjection = pVP.lock()->projection.get();
	//m_vColor = pVP.lock()->color.get();
	//m_vPos = pVP.lock()->position.get();
	//m_vLook = pVP.lock()->look.get();
	//m_vUp = pVP.lock()->up.get();
	//m_vSide = pVP.lock()->side.get();
	//m_vFront = pVP.lock()->front.get();
	//m_vParallelScale = pVP.lock()->parallelScale.get();
	//m_fNearClip = pVP.lock()->nearclip.get();
	//m_fFarClip = pVP.lock()->farclip.get();
	//m_fFov = pVP.lock()->fov.get();
	//m_isPerspective = pVP.lock()->perspective.get();
}

void ViewPoint::CalcReflect(const Vector3* pPoint, const Vector3* pNormal)
{
	Vector3 vNormal(0.f, 1.f, 0.f);
	Vector3 vPoint(0.f, 0.f, 0.f);
	if (pNormal) vNormal = *pNormal;
	if (pPoint) vPoint = *pPoint;

	Vector3 vA = position.get() + front.get() * farclip.get();
	Vector3 vB = position.get();
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

	m_vKeepLook = look.get();
	m_vKeepPos = position.get();
	Vector3 vBA = vB - vA;

	float fRatio = abs(dot_PA) / (abs(dot_PA) + abs(dot_PB));

	look.set(vA + (vBA * fRatio));

	Vector3 vDis = look.get() - position.get();
	vDis *= -1;
	float a = vDis.Dot(vNormal);
	vDis *= -1;
	position.set(vDis + vNormal * a * 2.0f);
	position.set(look.get() - position.get());
	m_mKeepView = view.get();
	CalcView(1.0f);
}

void ViewPoint::Restore()
{
	look.set(m_vKeepLook);
	position.set(m_vKeepPos);
	view.set(m_mKeepView);
}


void ViewPoint::BindRenderTarget()
{
	if (m_pRenderTarget.get())
	{
		DirectX11::GetInstance().BeginDraw(&m_pRenderTarget->GetViewPort(), m_pRenderTarget->GetRTV(), m_pRenderTarget->GetDepthStecil(), &m_pRenderTarget->GetClearColorList(), m_pRenderTarget->GetNum());
	}
}

//const FrustumType::kind ViewPoint::CollisionViewFrustum(const DirectX::XMFLOAT3& pos, const float fRadius)
//{
//	bool isHit = false;
//	DirectX::XMVECTOR vFrusW, vCenter, vDot;
//	float fDot;
//	vCenter = DirectX::XMLoadFloat3(&pos);
//
//	for (auto& itr : m_frustumWorld)
//	{
//		vFrusW = DirectX::XMLoadFloat4(&itr);
//		vDot = DirectX::XMPlaneDotCoord(vFrusW, vCenter);
//		DirectX::XMStoreFloat(&fDot, vDot);
//		if (fDot < -fRadius)
//			return FrustumType::OUTSIDE;	// 完全に外側
//		if (fDot <= fRadius)
//			isHit = true;
//	}
//	if (isHit)
//		return FrustumType::PARTLYINSIDE;	// 部分的に内側
//	return FrustumType::INSIDE;	// 完全に内側
//}

ID3D11ShaderResourceView * ViewPoint::GetRenderingTexture(int num)
{
	if (!m_pRenderTarget.get())nullptr;
	if (num > m_pRenderTarget->GetNum())nullptr;
	
	return m_pRenderTarget->GetSRV()[num];
}


void ViewPoint::CalcView(const float fUp)
{
	Vector3 vPos = position.get();
	Vector3 vLook = look.get();

	view.set(DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(vPos.x, vPos.y, vPos.z, 0),
		DirectX::XMVectorSet(vLook.x, vLook.y, vLook.z, 0),
		DirectX::XMVectorSet(0, fUp, 0, 0)
	));
}

void ViewPoint::CalcProjection()
{
	Vector2 vp = vpSize.get();

	if (perspective.get())
	{
		projection.set(DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(fov.get()), vp.x / vp.y, nearclip.get(), farclip.get()
		));
	}
	else
	{
		Vector2 halfSize = vp / 2.f / parallelScale.get();
		projection.set(DirectX::XMMatrixOrthographicOffCenterLH(
			-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, nearclip.get(), farclip.get()
		));
	}
}

void ViewPoint::CalcWorldMatrix()
{
	DirectX::XMFLOAT4X4 world;
	
	Vector3 vSide = side.get();
	Vector3 vUp = up.get();
	Vector3 vFront = front.get();
	Vector3 vPos = position.get();

	world._11 = vSide.x;
	world._12 = vSide.y;
	world._13 = vSide.z;
	world._14 = 0.0f;
	world._21 = vUp.x;
	world._22 = vUp.y;
	world._23 = vUp.z;
	world._24 = 0.0f;
	world._31 = vFront.x;
	world._32 = vFront.y;
	world._33 = vFront.z;
	world._34 = 0.0f;
	world._41 = vPos.x;
	world._42 = vPos.y;
	world._43 = vPos.z;
	world._44 = 1.0f;
}

//void ViewPoint::CreateViewFrustum()
//{
//	float fTan = tanf(DirectX::XMConvertToRadians(m_fFov) * 0.5f);
//	m_frustum[0] = DirectX::XMFLOAT4(0.0f, -1.0f, fTan, 0.0f);
//	m_frustum[1] = DirectX::XMFLOAT4(0.0f, 1.0f, fTan, 0.0f);
//	fTan *= GetAspect();
//	m_frustum[2] = DirectX::XMFLOAT4(1.0f, 0.0f, fTan, 0.0f);
//	m_frustum[3] = DirectX::XMFLOAT4(-1.0f, 0.0f, fTan, 0.0f);
//	m_frustum[4] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, -m_fNearClip);
//	m_frustum[5] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, m_fFarClip);
//	
//	for (int i = 0; i < 4; ++i)
//	{
//		DirectX::XMStoreFloat4(&m_frustum[i], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_frustum[i])));
//	}
//}
//
//void ViewPoint::UpdateViewFrustum()
//{
//	DirectX::XMMATRIX mtx = DirectX::XMLoadFloat4x4(&m_mWorldMatrix);
//	mtx = DirectX::XMMatrixInverse(nullptr, mtx);
//	mtx = DirectX::XMMatrixTranspose(mtx);
//	for (int i = 0; i < 6; ++i)
//	{
//		DirectX::XMStoreFloat4(&m_frustumWorld[i], DirectX::XMPlaneTransform(DirectX::XMLoadFloat4(&m_frustum[i]), mtx));
//	}
//}
