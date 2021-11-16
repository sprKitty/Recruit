#include "Collision.h"
#include <App/Component/Collider.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>

const int DIVISION = 300;

void Collision::Uninit()
{
	if (!m_pColliderList.empty())
		m_pColliderList.clear();
}

void Collision::Update()
{
	int nSkip = -1;
	for (auto pA : m_pColliderList)
	{
		++nSkip;
		if (!pA->m_isActive)
			continue;
		int nContinue = -1;
		for (auto pB : m_pColliderList)
		{
			++nContinue;
			if (nContinue < nSkip + 1)
				continue;
			if (!pB->m_isActive)
				continue;
			//if (pA->m_pOwner->GetObjectType() == OBJ_BULLET && pB->m_pOwner->GetObjectType() == OBJ_BEAR)
			//{
			//	if (RaySegment(pA, pB))
			//	{
			//		pA->EnableHit();
			//		pB->EnableHit();
			//	}
			//	else
			//	{
			//		pA->DisableHit();
			//		pB->DisableHit();
			//	}
			//}
			//else if (pB->m_pOwner->GetObjectType() == OBJ_BULLET && pA->m_pOwner->GetObjectType() == OBJ_BEAR)
			//{
			//	if (RaySegment(pB, pA))
			//	{
			//		pA->EnableHit();
			//		pB->EnableHit();
			//	}
			//	else
			//	{
			//		pA->DisableHit();
			//		pB->DisableHit();
			//	}
			//}
			//else if (pA->m_pOwner->GetObjectType() == OBJ_PLAYER && pB->m_pOwner->GetObjectType() == OBJ_BEAR || pB->m_pOwner->GetObjectType() == OBJ_PLAYER && pA->m_pOwner->GetObjectType() == OBJ_BEAR)
			//{
			if (OBBvsOBB(pA, pB))
			{
				pA->EnableHit();
				pB->EnableHit();
			}
			else
			{
				pA->DisableHit();
				pB->DisableHit();
			}
			//}
		}
	}
}

//void Collision::Draw()
//{
//	if (DEBUG)
//	{
//		ShaderBuffer::GetInstance().SetColor(DirectX::XMFLOAT4(1.0f, 0, 0, 0.5f));
//		ShaderBuffer::GetInstance().BindVS(VS_TYPE::NORMAL);
//		ShaderBuffer::GetInstance().BindPS(PS_TYPE::COLOR);
//		for (auto itr : m_pColliderList)
//		{
//			itr->Draw();
//		}
//	}
//}

bool Collision::OBB(Collider * pA, Collider * pB)
{
	// Colliderコンポーネントのワールド座標を取得
	DirectX::XMFLOAT4X4 wA = pA->GetWorld();
	DirectX::XMFLOAT4X4 wB = pB->GetWorld();

	Vector3 NAe1 = MyMath::Normalize(Vector3(wA._11, wA._12, wA._13));
	Vector3 NAe2 = MyMath::Normalize(Vector3(wA._21, wA._22, wA._23));
	Vector3 NAe3 = MyMath::Normalize(Vector3(wA._31, wA._32, wA._33));
	Vector3 NBe1 = MyMath::Normalize(Vector3(wB._11, wB._12, wB._13));
	Vector3 NBe2 = MyMath::Normalize(Vector3(wB._21, wB._22, wB._23));
	Vector3 NBe3 = MyMath::Normalize(Vector3(wB._31, wB._32, wB._33));

	Vector3 Ae1 = NAe1 * wA._11 * 0.5f;
	Vector3 Ae2 = NAe2 * wA._22 * 0.5f;
	Vector3 Ae3 = NAe3 * wA._33 * 0.5f;
	Vector3 Be1 = NBe1 * wB._11 * 0.5f;
	Vector3 Be2 = NBe2 * wB._22 * 0.5f;
	Vector3 Be3 = NBe3 * wB._33 * 0.5f;

	Vector3 Interval = Vector3(wA._41 - wB._41, wA._42 - wB._42, wA._43 - wB._43);

	float rA, rB, L;
	
	// 分離軸 : Ae1
	rA = MyMath::Length(&Ae1);
	rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae2
	rA = MyMath::Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = MyMath::Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = MyMath::Length(&Be1);
	L = fabsf(MyMath::Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = MyMath::Length(&Be2);
	L = fabsf(MyMath::Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = MyMath::Length(&Be3);
	L = fabsf(MyMath::Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	Vector3 cross;
	
	// 分離軸 : C11
	cross = MyMath::Cross(&NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	cross = MyMath::Cross(&NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	cross = MyMath::Cross(&NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	cross = MyMath::Cross(&NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	cross = MyMath::Cross(&NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	cross = MyMath::Cross(&NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	cross = MyMath::Cross(&NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&cross, &Be2, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	cross = MyMath::Cross(&NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be3);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	cross = MyMath::Cross(&NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&cross, &Be1, &Be2);
	L = fabsf(MyMath::Dot(&Interval, &cross));
	if (L > rA + rB)
		return false;

	return true;
}


void Collision::Delete(Collider * pCollider)
{
	for (auto pCol = m_pColliderList.begin(); pCol != m_pColliderList.end();)
	{
		if (*pCol == pCollider)
		{
			pCol = m_pColliderList.erase(pCol);
		}
		if (pCol == m_pColliderList.end())
			break;
		++pCol;
	}
}

//bool Collision::RaySegment(Collider * pA, Collider * pB)
//{
//	if (!pA->m_isActive || !pB->m_isActive)
//		return false;
//	DirectX::XMFLOAT4X4 wA = pA->GetWorld();
//	Transform tA = pA->m_pOwner->GetTransform();
//	DirectX::XMMATRIX mtxB = pB->GetWorldMatrix();
//	Vector3 vP0 = Vector3(wA._41, wA._42, wA._43);
//	Vector3 vOldPos = vP0 - tA.move;
//	Vector3 dis = vP0 - vOldPos;
//	vP0 = vOldPos;
//	float length = MyMath::Length(dis);
//	Vector3 normal = dis / length;
//	std::vector<Vector3> vtxList;
//	for (int i = 0; i < 36; ++i)
//	{
//		DirectX::XMMATRIX mtx = MyMath::ConvertMatrix(1, 0, Boxel::m_VtxList[pB->m_BoxType][i]);
//		mtx *= mtxB;
//		DirectX::XMFLOAT4X4 w;
//		DirectX::XMStoreFloat4x4(&w, mtx);
//		vtxList.push_back(Vector3(w._41, w._42, w._43));
//	}
//
//	bool isHit = false;
//	std::vector<DirectX::XMFLOAT3> posList;
//	for (int i = 0; i < 36; i += 3)
//	{
//		Vector3 V0, V1, V2;
//		V0 = vtxList[i + 0];
//		V1 = vtxList[i + 1];
//		V2 = vtxList[i + 2];
//		DirectX::XMFLOAT3 V0V1 = MyMath::V3ConvertDirectX::XM3(V1 - V0);
//		DirectX::XMFLOAT3 V1V2 = MyMath::V3ConvertDirectX::XM3(V2 - V1);
//		DirectX::XMFLOAT3 V2V0 = MyMath::V3ConvertDirectX::XM3(V0 - V2);
//
//		DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&V0V1), DirectX::XMLoadFloat3(&V1V2)));
//		// tの式(t=(N・(V0-P0))/(N・W))の分母を求める
//		float base;
//		DirectX::XMStoreFloat(&base, DirectX::XMVector3Dot(N, DirectX::XMLoadFloat3(&MyMath::V3ConvertDirectX::XM3(normal))));
//		if (base == 0.0f)
//			continue; // 面とレイが平行
//		// 分子を求める
//		DirectX::XMFLOAT3 P0V0(V0.x - vP0.x, V0.y - vP0.y, V0.z - vP0.z);
//		float dot;
//		DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(N,
//			DirectX::XMLoadFloat3(&P0V0)));
//		// tから交点を求める
//		float t = dot / base;
//		if (t < 0.0f)
//			continue; // 交点が始点の後ろ
//		DirectX::XMFLOAT3 X(vP0.x + t * normal.x,
//			vP0.y + t * normal.y, vP0.z + t * normal.z);
//		// 内外判定
//		DirectX::XMFLOAT3 V0X(X.x - V0.x, X.y - V0.y, X.z - V0.z);
//		DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(N, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&V0V1), DirectX::XMLoadFloat3(&V0X))));
//		if (dot < 0.0f)
//			continue;
//		DirectX::XMFLOAT3 V1X(X.x - V1.x, X.y - V1.y, X.z - V1.z);
//		DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(N, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&V1V2), DirectX::XMLoadFloat3(&V1X))));
//		if (dot < 0.0f)
//			continue;
//		DirectX::XMFLOAT3 V2X(X.x - V2.x, X.y - V2.y, X.z - V2.z);
//		DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(N, DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&V2V0), DirectX::XMLoadFloat3(&V2X))));
//		if (dot < 0.0f)
//			continue;
//
//		//pA->m_isActive = false;
//		//pB->SetHitPos(normal);
//		//return true;
//
//		isHit = true;
//		posList.push_back(X);
//	}
//
//	if (isHit)
//	{
//		int number;
//		float length2;
//		for (int i = 0; i < posList.size(); ++i)
//		{
//			if (i == 0)
//			{
//				length = MyMath::Length((vP0 - MyMath::DirectX::XM3ConvertV3(posList[i])));
//				number = i;
//			}
//			else
//			{
//				length2 = MyMath::Length((vP0 - MyMath::DirectX::XM3ConvertV3(posList[i])));
//				if (length2 < length)
//				{
//					length = length2;
//					number = i;
//				}
//			}
//
//		}
//		pA->m_isActive = false;
//		pB->SetHitPos(MyMath::DirectX::XM3ConvertV3(posList[number]));
//		return true;
//	}
//
//	return false;
//}


bool Collision::OBBvsOBB(Collider * pA, Collider * pB)
{
	// Colliderコンポーネントのワールド座標を取得
	DirectX::XMFLOAT4X4 wA = pA->GetWorld();
	DirectX::XMFLOAT4X4 wB = pB->GetWorld();

	DirectX::XMFLOAT3 center = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT3 vP1, vP2;
	DirectX::XMStoreFloat3(&vP1, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&center), DirectX::XMLoadFloat4x4(&wA)));
	DirectX::XMStoreFloat3(&vP2, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&center), DirectX::XMLoadFloat4x4(&wB)));

	DirectX::XMVECTOR vD;
	vD = DirectX::XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);

	DirectX::XMVECTOR vN[6];
	vN[0] = DirectX::XMVectorSet(wA._11, wA._12, wA._13, 0.0f);
	vN[1] = DirectX::XMVectorSet(wA._21, wA._22, wA._23, 0.0f);
	vN[2] = DirectX::XMVectorSet(wA._31, wA._32, wA._33, 0.0f);
	vN[3] = DirectX::XMVectorSet(wB._11, wB._12, wB._13, 0.0f);
	vN[4] = DirectX::XMVectorSet(wB._21, wB._22, wB._23, 0.0f);
	vN[5] = DirectX::XMVectorSet(wB._31, wB._32, wB._33, 0.0f);

	DirectX::XMVECTOR vL[6];
	vL[0] = DirectX::XMVectorSet(wA._11 * 0.5f, wA._12 * 0.5f, wA._13 * 0.5f, 0.0f);
	vL[1] = DirectX::XMVectorSet(wA._21 * 0.5f, wA._22 * 0.5f, wA._23 * 0.5f, 0.0f);
	vL[2] = DirectX::XMVectorSet(wA._31 * 0.5f, wA._32 * 0.5f, wA._33 * 0.5f, 0.0f);
	vL[3] = DirectX::XMVectorSet(wB._11 * 0.5f, wB._12 * 0.5f, wB._13 * 0.5f, 0.0f);
	vL[4] = DirectX::XMVectorSet(wB._21 * 0.5f, wB._22 * 0.5f, wB._23 * 0.5f, 0.0f);
	vL[5] = DirectX::XMVectorSet(wB._31 * 0.5f, wB._32 * 0.5f, wB._33 * 0.5f, 0.0f);

	float fL, f, fD;
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMVECTOR& vS = vN[i];	// 分離軸候補
		// OBBの影(の半分)の合計
		fL = 0.0f;
		for (int j = 0; j < 6; ++j)
		{
			DirectX::XMStoreFloat(&f, DirectX::XMVector3Dot(vS, vL[j]));
			fL += fabsf(f);
		}
		// 中心座標の距離と比較
		DirectX::XMStoreFloat(&fD, DirectX::XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD)
			return false;
	}
	// 分離軸候補がそれぞれの辺に垂直なベクトル
	DirectX::XMVECTOR vS;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j < 6; ++j)
		{
			// 分離軸を求める
			vS = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vN[i], vN[j]));

			// OBBの影(の半分)の合計
			fL = 0.0f;
			for (int k = 0; k < 6; ++k)
			{
				DirectX::XMStoreFloat(&f, DirectX::XMVector3Dot(vS, vL[k]));
				fL += fabsf(f);
			}

			// 中心座標の距離と比較
			DirectX::XMStoreFloat(&fD, DirectX::XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD)
				return false;
		}
	}
	return true;
}

float Collision::LenSegOnSeparateAxis(Vector3 * pSep, Vector3* pE1, Vector3* pE2, Vector3* pE3)
{
	float r1 = fabsf(MyMath::Dot(pSep, pE1));
	float r2 = fabsf(MyMath::Dot(pSep, pE2));
	float r3 = pE3 ? (fabsf(MyMath::Dot(pSep, pE3))) : 0;
	return r1 + r2 + r3;
}
