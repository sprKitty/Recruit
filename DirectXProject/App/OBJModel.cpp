#include "OBJModel.h"
#include <App/Component/Mesh.h>
#include <System/OBJParser.h>
#include <Shader/ShaderBuffer.h>
#include <System/Texture.h>
#include <System/DebugLog.h>

OBJModel::OBJModel()
	:m_pBuffer(nullptr)
{
}

OBJModel::~OBJModel()
{
	if (m_pBuffer) delete m_pBuffer;
}

const MeshData::Info OBJModel::Load(const std::string& path, const std::string& name)
{
	HRESULT hr;
	NewOBJParser obj;
	MeshData::Info info;

	if (!obj.Load(path, name))
	{
		return info;
	}
	info.pDrawBuffer.reset(new DrawBuffer());

	std::vector<Vector3> vtxList = obj.GetVtxList();
	std::vector<Vector2> uvList = obj.GetUVList();
	std::vector<Vector3> normalList = obj.GetNormalList();
	std::map<std::string, NewOBJParser::Material> materialMap = obj.GetMaterialMap();
	std::map<std::string, std::vector<VectorInt3>> indexMap = obj.GetIndexMap();

	size_t vtxNum = 0;

	for (const auto& itr : indexMap)
	{
		vtxNum += itr.second.size();
	}
	VertexBuffer* pVtxBuffer = new VertexBuffer[vtxNum];
	UINT16 i(0);

	for (const auto& itrIndex : indexMap)
	{
		auto itrMat = materialMap[itrIndex.first];
		for (const auto& itr : itrIndex.second)
		{
			pVtxBuffer[i].vtx.pos = vtxList[itr.x - 1];
			pVtxBuffer[i].vtx.uv = uvList[itr.y - 1];
			pVtxBuffer[i].vtx.normal = normalList[itr.z - 1];
			pVtxBuffer[i].vtx.tangent = 0;
			pVtxBuffer[i].ambient = itrMat.Ka;
			pVtxBuffer[i].diffuse = itrMat.Kd;
			pVtxBuffer[i].specular = itrMat.Ks;
			pVtxBuffer[i].specularIndex = itrMat.Ns;
			++i;
		}
	}

	for (int i = 0; i < vtxNum; i += 3)
	{
		Mesh::Surface surface;
		Vector3 vTangent = MyMath::CalcTangent(pVtxBuffer[i].vtx.pos, pVtxBuffer[i].vtx.uv, pVtxBuffer[i + 1].vtx.pos, pVtxBuffer[i + 1].vtx.uv, pVtxBuffer[i + 2].vtx.pos, pVtxBuffer[i + 2].vtx.uv);

		pVtxBuffer[i].vtx.tangent = vTangent;
		pVtxBuffer[i + 1].vtx.tangent = vTangent;
		pVtxBuffer[i + 2].vtx.tangent = vTangent;
		surface.vPos0 = pVtxBuffer[i].vtx.pos;
		surface.vPos1 = pVtxBuffer[i + 1].vtx.pos;
		surface.vPos2 = pVtxBuffer[i + 2].vtx.pos;
		surface.vNormal = pVtxBuffer[i].vtx.normal;
		info.surfaceList.emplace_back(surface);
	}

	hr = info.pDrawBuffer->CreateVertexBuffer(pVtxBuffer, sizeof(VertexBuffer), vtxNum);
	
	if (FAILED(hr))
	{
		DebugLog::GetInstance().FreeError("キャラクター用ポリ生成失敗");
		info.pDrawBuffer.reset();
		info.surfaceList.clear();
	}
	delete[] pVtxBuffer;
	return info;
}

void OBJModel::Draw()
{
	m_pBuffer->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
