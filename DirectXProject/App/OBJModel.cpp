#include "OBJModel.h"
#include <App/Component/Mesh.h>
#include <System/OBJParser.h>
#include <Shader/ShaderBuffer.h>
#include <System/Texture.h>
#include <System/DebugLog.h>

OBJModel::OBJModel()
	:m_pBuffer(nullptr)
	, m_pVtxList(nullptr)
	, m_nVtxNum(0)
{
}

OBJModel::~OBJModel()
{
	if (m_pBuffer) delete m_pBuffer;
	if (m_pVtxList) delete[] m_pVtxList;
}

const MeshData::Info OBJModel::Load(const char * pPath)
{
	HRESULT hr;
	OBJParser obj;
	MeshData::Info info;
	if (!obj.Load(pPath))
	{
		return info;
	}
	info.pDrawBuffer.reset(new DrawBuffer());

	std::vector<Vector3> posList = obj.GetVertexList();
	std::vector<Vector2> uvList = obj.GetUVList();
	std::vector<Vector3> normalList = obj.GetNormalList();
	std::vector<int> posIdxList = obj.GetVertexIndexList();
	std::vector<int> uvIdxList = obj.GetUVIndexList();
	std::vector<int> normalIdxList = obj.GetNormalIndexList();

	int nVtxNum = posIdxList.size();
	MyMath::Vertex* pVtxList = new MyMath::Vertex[nVtxNum];

	for (int i = 0; i < nVtxNum; ++i)
	{
		pVtxList[i].pos = posList[posIdxList[i] - 1];
		pVtxList[i].uv = uvList[uvIdxList[i] - 1];
		pVtxList[i].normal = normalList[normalIdxList[i] - 1];
		pVtxList[i].tangent = 0;
		pVtxList[i].binormal = 0;
	}

	for (int i = 0; i < nVtxNum; i += 3)
	{
		Vector3 vTangent, vBinormal;
		Mesh::Surface surface;
		if (MyMath::TangentandBinormal(pVtxList[i], pVtxList[i + 1], pVtxList[i + 2], &vTangent, &vBinormal))
		{
			pVtxList[i].tangent = vTangent;
			pVtxList[i + 1].tangent = vTangent;
			pVtxList[i + 2].tangent = vTangent;
			pVtxList[i].binormal = vBinormal;
			pVtxList[i + 1].binormal = vBinormal;
			pVtxList[i + 2].binormal = vBinormal;
		}
		surface.vPos0 = pVtxList[i].pos;
		surface.vPos1 = pVtxList[i + 1].pos;
		surface.vPos2 = pVtxList[i + 2].pos;
		surface.vNormal = pVtxList[i].normal;
		info.surfaceList.emplace_back(surface);
	}

	hr = info.pDrawBuffer->CreateVertexBuffer(pVtxList, sizeof(MyMath::Vertex), nVtxNum);
	
	if (FAILED(hr))
	{
		DebugLog::GetInstance().FreeError("キャラクター用ポリ生成失敗");
		info.pDrawBuffer.reset();
		info.surfaceList.clear();
	}
	delete[] pVtxList;
	return info;
}

bool OBJModel::Create(const char * fileName)
{
	bool result = true;

	OBJParser obj;
	result = obj.Load(fileName);
	if (!result) { return result; }

	m_pBuffer = new DrawBuffer();

	std::vector<Vector3> posList = obj.GetVertexList();
	std::vector<Vector2> uvList = obj.GetUVList();
	std::vector<Vector3> normalList = obj.GetNormalList();
	std::vector<int> posIdxList = obj.GetVertexIndexList();
	std::vector<int> uvIdxList = obj.GetUVIndexList();
	std::vector<int> normalIdxList = obj.GetNormalIndexList();
	m_nVtxNum = posIdxList.size();
	m_pVtxList = new MyMath::Vertex[m_nVtxNum];

	for (int i = 0; i < m_nVtxNum; ++i)
	{
		m_pVtxList[i].pos  = posList[posIdxList[i] - 1];
		m_pVtxList[i].uv = uvList[uvIdxList[i] - 1];
		m_pVtxList[i].normal = normalList[normalIdxList[i] - 1];
		m_pVtxList[i].tangent = 0;
		m_pVtxList[i].binormal = 0;
	}

	for (int i = 0; i < m_nVtxNum; i += 3)
	{
		Vector3 vTangent,vBinormal;
		if (MyMath::TangentandBinormal(m_pVtxList[i], m_pVtxList[i + 1], m_pVtxList[i + 2], &vTangent, &vBinormal))
		{
			m_pVtxList[i].tangent = vTangent;
			m_pVtxList[i + 1].tangent = vTangent;
			m_pVtxList[i + 2].tangent = vTangent;
			m_pVtxList[i].binormal = vBinormal;
			m_pVtxList[i + 1].binormal = vBinormal;
			m_pVtxList[i + 2].binormal = vBinormal;
		}
	}

	m_pBuffer->CreateVertexBuffer(m_pVtxList, sizeof(MyMath::Vertex), m_nVtxNum);

	return false;
}

void OBJModel::Draw()
{
	m_pBuffer->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
