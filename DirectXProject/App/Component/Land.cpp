#include "Land.h"
#include <App/Component/Object.h>
#include <App/Component/Renderer.h>
#include <System/Texture.h>
#include <Shader/ShaderBuffer.h>

Land* Land::m_pLand;

constexpr float g_fQuadMAX_X = 1000.0f;
constexpr float g_fQuadMAX_Z = 1000.0f;
constexpr int g_nSplitNum_X = 10;
constexpr int g_nSplitNum_Z = 10;

Land::Land()
{
	m_pTex = nullptr;
	m_pTex1 = nullptr;
	m_pBump0 = nullptr;
	m_pBump1 = nullptr;
}


Land::~Land()
{
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pTex1);
	SAFE_RELEASE(m_pBump0);
	SAFE_RELEASE(m_pBump1);
}

void Land::Init()
{
	m_pLand = this;
	LoadTextureFromFile("Assets/Ground.png", &m_pTex);
	LoadTextureFromFile("Assets/renga.png", &m_pTex1);
	//LoadTextureFromFile("Assets/iwahada.jpeg", &m_pTex);
	LoadTextureFromFile("Assets/BumpGround.jpg", &m_pBump0);
	LoadTextureFromFile("Assets/Bumprenga.png", &m_pBump1);
	//LoadTextureFromFile("Assets/Bumpiwahada.jpeg", &m_pBump0);
	Transform &t = m_pOwner->AddressTransform();
	t.pos.y = 0;
	t.scale = 20;
	m_pOwner->AddComponent<Renderer>()->SetModel("Assets/OBJ/field.obj");
	m_pOwner->GetComponent<Renderer>()->DrawShadow();
	CreateQuadTree();
}

void Land::Uninit()
{
	for (int i = 0; i < m_pSquares.size(); ++i)
	{
		delete m_pSquares[i];
	}
	m_pSquares.clear();
}


void Land::Update()
{
	SHADER->SetTexture(m_pTex1, ShaderResource::TEX_TRILANAR_X);
	SHADER->SetTexture(m_pTex, ShaderResource::TEX_TRILANAR_Y);
	SHADER->SetTexture(m_pTex1, ShaderResource::TEX_TRILANAR_Z);

	SHADER->SetTexture(m_pBump1, ShaderResource::TEX_BUMP_X);
	SHADER->SetTexture(m_pBump1, ShaderResource::TEX_BUMP_Y);
	SHADER->SetTexture(m_pBump1, ShaderResource::TEX_BUMP_Z);
}


void Land::Draw()
{
}


bool Land::Collision(Vector3 vP0, Vector3 vW, XMFLOAT3 * pX, XMFLOAT3 * pN)
{
	for (int i = 0; i < m_pSquares.size(); ++i)
	{
		if (vP0.x >= m_pSquares[i]->min_x &&
			vP0.x < m_pSquares[i]->max_x &&
			vP0.z >= m_pSquares[i]->min_z &&
			vP0.z < m_pSquares[i]->max_z)
		{
			for (int j = 0; j < m_pSquares[i]->vtxList.size(); j += 3)
			{
				Vector3 V0, V1, V2;
				V0 = m_pSquares[i]->vtxList[j + 0];
				V1 = m_pSquares[i]->vtxList[j + 1];
				V2 = m_pSquares[i]->vtxList[j + 2];
				XMFLOAT3 V0V1 = MyMath::V3ConvertXM3(V1 - V0);
				XMFLOAT3 V1V2 = MyMath::V3ConvertXM3(V2 - V1);
				XMFLOAT3 V2V0 = MyMath::V3ConvertXM3(V0 - V2);

				XMVECTOR N = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&V0V1), XMLoadFloat3(&V1V2)));
				// tの式(t=(N・(V0-P0))/(N・W))の分母を求める
				float base;
				XMStoreFloat(&base, XMVector3Dot(N, XMLoadFloat3(&MyMath::V3ConvertXM3(vW))));
				if (base == 0.0f)
					continue; // 面とレイが平行
				// 分子を求める
				XMFLOAT3 P0V0(V0.x - vP0.x, V0.y - vP0.y, V0.z - vP0.z);
				float dot;
				XMStoreFloat(&dot, XMVector3Dot(N,
					XMLoadFloat3(&P0V0)));
				// tから交点を求める
				float t = dot / base;
				if (t < 0.0f)
					continue; // 交点が始点の後ろ
				XMFLOAT3 X(vP0.x + t * vW.x,
					vP0.y + t * vW.y, vP0.z + t * vW.z);
				// 内外判定
				XMFLOAT3 V0X(X.x - V0.x, X.y - V0.y, X.z - V0.z);
				XMStoreFloat(&dot, XMVector3Dot(N, XMVector3Cross(XMLoadFloat3(&V0V1), XMLoadFloat3(&V0X))));
				if (dot < 0.0f)
					continue;
				XMFLOAT3 V1X(X.x - V1.x, X.y - V1.y, X.z - V1.z);
				XMStoreFloat(&dot, XMVector3Dot(N, XMVector3Cross(XMLoadFloat3(&V1V2), XMLoadFloat3(&V1X))));
				if (dot < 0.0f)
					continue;
				XMFLOAT3 V2X(X.x - V2.x, X.y - V2.y, X.z - V2.z);
				XMStoreFloat(&dot, XMVector3Dot(N, XMVector3Cross(XMLoadFloat3(&V2V0), XMLoadFloat3(&V2X))));
				if (dot < 0.0f)
					continue;
				// 当たり
				if
					(pX) *pX = X;
				if
					(pN) XMStoreFloat3(pN, N);
				return true;// 当たっている
			}
			return false;
		}
	}
	return false;
}


void Land::CreateQuadTree()
{
	std::vector<Vector3> wVtxList;
	int vtxNum = m_pOwner->GetComponent<Renderer>()->GetOBJModel()->GetVtxNum();
	MyMath::Vertex* vtxList = m_pOwner->GetComponent<Renderer>()->GetOBJModel()->GetVtxList();
	XMMATRIX mtx = m_pOwner->GetWorldMatrix();
	for (int i = 0; i < vtxNum; ++i)
	{
		XMMATRIX mtxVtx = MyMath::ConvertMatrix(1, 0, vtxList[i].pos);
		mtxVtx *= mtx;
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, mtxVtx);
		wVtxList.push_back(Vector3(mW._41, mW._42, mW._43));
	}

	constexpr float quad_x = g_fQuadMAX_X * 2;
	constexpr float quad_z = g_fQuadMAX_Z * 2;
	constexpr float split_x = quad_x / g_nSplitNum_X;
	constexpr float split_z = quad_z / g_nSplitNum_Z;
	constexpr float size_x = split_x / 2.0f;
	constexpr float size_z = split_z / 2.0f;

	for (int i = 0; i < g_nSplitNum_X; ++i)
	{
		float vtx_x = -g_fQuadMAX_X + split_x * i;
		for (int j = 0; j < g_nSplitNum_Z; ++j)
		{
			float vtx_z = -g_fQuadMAX_Z + split_z * j;
			Square* pSquare = new Square;
			pSquare->max_x = vtx_x + size_x;
			pSquare->min_x = vtx_x - size_x;
			pSquare->max_z = vtx_z + size_z;
			pSquare->min_z = vtx_z - size_z;
			m_pSquares.push_back(pSquare);
		}
	}

	for (int i = 0; i < wVtxList.size(); i += 3)
	{
		Vector3 V[3];
		V[0] = wVtxList[i + 0];
		V[1] = wVtxList[i + 1];
		V[2] = wVtxList[i + 2];
		int num[2] = { -1,-1 };
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < m_pSquares.size(); ++k)
			{
				if (V[j].x >= m_pSquares[k]->min_x &&
					V[j].x < m_pSquares[k]->max_x &&
					V[j].z >= m_pSquares[k]->min_z &&
					V[j].z < m_pSquares[k]->max_z)
				{
					switch (j)
					{
					case 0:
						m_pSquares[k]->vtxList.push_back(V[0]);
						m_pSquares[k]->vtxList.push_back(V[1]);
						m_pSquares[k]->vtxList.push_back(V[2]);
						num[0] = k;
						break;
					case 1:
						if (num[0] != k)
						{
							m_pSquares[k]->vtxList.push_back(V[0]);
							m_pSquares[k]->vtxList.push_back(V[1]);
							m_pSquares[k]->vtxList.push_back(V[2]);
							num[1] = k;
						}
						break;
					case 2:
						if (num[0] != k && num[1] != k)
						{
							m_pSquares[k]->vtxList.push_back(V[0]);
							m_pSquares[k]->vtxList.push_back(V[1]);
							m_pSquares[k]->vtxList.push_back(V[2]);
						}
						break;
					default:
						break;
					}
					break;
				}
			}
		}
	}
}
