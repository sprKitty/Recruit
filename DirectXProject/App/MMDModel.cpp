#include <App/MMDModel.h>
#include <System/MMDParser.h>
#include <System/Texture.h>
#include <Shader/ShaderBuffer.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


MMDModel::MMDModel()
	: m_pBuffer(nullptr)
	, m_pVertex(nullptr), m_vtxNum(0)
	, m_ppTexture(nullptr)
	, m_pMaterial(nullptr), m_materialNum(0)
	, m_nTexNum(0)
	//, m_pBone(nullptr), m_boneNum(0)
{
}


MMDModel::~MMDModel()
{
	if (m_pBuffer) { delete m_pBuffer; }
	if (m_pVertex) { delete[] m_pVertex; }
	if (m_pMaterial) { delete[] m_pMaterial; }
	for (int i = 0; i < m_nTexNum; ++i)
	{
		SAFE_RELEASE(m_ppTexture[i]);
	}
	//if (m_pBone) { delete[] m_pBone; }
	//if (m_bth.afterPhysics->pList)
	//{
	//	delete[] m_bth.afterPhysics->pList;
	//}
	//if (m_bth.prePhysics->pList)
	//{
	//	delete[] m_bth.prePhysics->pList;
	//}
	//if (m_bth.afterPhysics) 
	//{
	//	delete[] m_bth.afterPhysics;
	//}
	//if (m_bth.prePhysics)
	//{
	//	delete[] m_bth.prePhysics;
	//}
}


bool MMDModel::Create(const char * fileName)
{
	MMDParser mmd;
	if (!mmd.Load(fileName))
	{
		return false;
	}
	// 頂点データのコピー
	m_vtxNum = mmd.GetVertexNum();
	m_pVertex = new VertexInfo[m_vtxNum];
	MMDParser::VertexInfo* pVtx = mmd.GetVertex();
	for (int i = 0; i < m_vtxNum; ++i)
	{
		m_pVertex[i].pos = pVtx[i].m_pos;
		m_pVertex[i].normal = pVtx[i].m_normal;
		m_pVertex[i].uv = pVtx[i].m_uv;
		m_pVertex[i].tangent = DirectX::XMFLOAT3(0, 0, 0);
		m_pVertex[i].binormal = DirectX::XMFLOAT3(0, 0, 0);
	}
	// インデックスの取得
	int idxNum = mmd.GetIndexNum();
	char idxSize = mmd.GetIndexSize();
	MMDParser::Index* pIdx = mmd.GetIndex();
	char* pIdxWork = new char[idxSize * idxNum];
	for (int i = 0; i < idxNum; ++i)
	{
		switch (idxSize)
		{
		case 1:
			*(((char*)pIdxWork) + i) = pIdx[i]._char;
			break;
		case 2:
			*(((short*)pIdxWork) + i) = pIdx[i]._short;
			break;
		case 4:
			*(((long*)pIdxWork) + i) = pIdx[i]._long;
			break;
		default:
			break;
		}
	}
	// バッファの作成
	m_pBuffer = new DrawBuffer();
	m_pBuffer->CreateVertexBuffer(
		m_pVertex, sizeof(VertexInfo), m_vtxNum
	);
	m_pBuffer->CreateIndexBuffer(
		pIdxWork, idxSize, idxNum);
	// テクスチャ読み込み
	char texPath[MAX_PATH];
	int pathEnd = PathFindFileName(fileName) - fileName;
	memcpy(texPath, fileName, pathEnd);
	int texNum = mmd.GetTextureNum();
	m_nTexNum = texNum;
	MMDParser::TextBuf* pTex = mmd.GetTexture();
	m_ppTexture = new ID3D11ShaderResourceView*[texNum];
	for (int i = 0; i < texNum; ++i)
	{
		texPath[pathEnd] = '\0';
		strcat(texPath, pTex[i].pBuf);
		LoadTextureFromFile(texPath, &m_ppTexture[i]);
	}
	// マテリアル
	m_materialNum = mmd.GetMaterialNum();
	m_pMaterial = new MaterialInfo[m_materialNum];
	int matIdxSize = mmd.GetMaterialIndexSize();
	int texIndex;
	MMDParser::Material* pMat = mmd.GetMaterial();
	for (int i = 0; i < m_materialNum; ++i)
	{
		switch (matIdxSize)
		{
		case 1: texIndex = pMat[i].texture._char; break;
		case 2: texIndex = pMat[i].texture._short; break;
		case 4: texIndex = pMat[i].texture._long; break;
		}
		m_pMaterial[i].texture = texIndex;
		m_pMaterial[i].faceNum = pMat[i].faceNum;
	}
	// ボーン
	//char boneIdxSize = mmd.GetBoneIndexSize();
	//m_boneNum = mmd.GetBoneNum();
	//MMDParser::Bone* pBone = mmd.GetBone();
	//m_pBone = new BoneInfo[m_boneNum];
	//for (int i = 0; i < m_boneNum; ++i)
	//{
	//	// TODO:本来はインデックスで分けて読み込む
	//	m_pBone[i].parent = pBone[i].parent._short;
	//	// 親と自分の座標から相対座標(移動量)を求める
	//	DirectX::XMVECTOR vStart =
	//		DirectX::XMVectorSet(0, 0, 0, 0);
	//	DirectX::XMVECTOR vEnd;
	//	if (m_pBone[i].parent >= 0)
	//	{
	//		vStart = DirectX::XMLoadFloat3(
	//			&pBone[m_pBone[i].parent].pos);
	//	}
	//	vEnd = DirectX::XMLoadFloat3(
	//		&pBone[i].pos);
	//	DirectX::XMStoreFloat3(
	//		&m_pBone[i].offset,
	//		DirectX::XMVectorSubtract(vEnd, vStart)
	//	);
	//	m_pBone[i].rotate = DirectX::XMFLOAT3(0, 0, 0);
	//}
	//// 変形階層ごとの処理順データ構築
	//m_bth.layerNum = 0;
	//for (int i = 0; i < m_boneNum; ++i)
	//{
	//	m_bth.layerNum = max(
	//		m_bth.layerNum,
	//		pBone[i].hierarchy + 1);
	//}
	//m_bth.prePhysics =
	//	new BoneTransformHierarchy::List[
	//		m_bth.layerNum];
	//m_bth.afterPhysics =
	//	new BoneTransformHierarchy::List[
	//		m_bth.layerNum];
	//// 階層ごとにインデックスのデータが
	//// いくつ格納されるかカウント
	//for (int i = 0; i < m_boneNum; ++i)
	//{
	//	// 物理後
	//	if (pBone[i].flg & 0x1000)
	//	{
	//		m_bth.afterPhysics[
	//			pBone[i].hierarchy].num++;
	//	}
	//	// 物理前
	//	else
	//	{
	//		m_bth.prePhysics[
	//			pBone[i].hierarchy].num++;
	//	}
	//}
	//// 階層ごとに格納配列を確保
	//for (int i = 0; i < m_bth.layerNum; ++i)
	//{
	//	if (m_bth.afterPhysics[i].num)
	//	{
	//		m_bth.afterPhysics[i].pList =
	//			new BoneInfo*[
	//				m_bth.afterPhysics[i].num
	//			];
	//		// この後、配列の先頭(0番)から順番に
	//		// データを格納するため、numを初期化
	//		m_bth.afterPhysics[i].num = 0;
	//	}
	//	if (m_bth.prePhysics[i].num)
	//	{
	//		m_bth.prePhysics[i].pList =
	//			new BoneInfo*[
	//				m_bth.prePhysics[i].num
	//			];
	//		m_bth.prePhysics[i].num = 0;
	//	}
	//}
	//// 処理順データ作成
	//for (int i = 0; i < m_boneNum; ++i)
	//{
	//	int layer = pBone[i].hierarchy;
	//	if (pBone[i].flg & 0x1000)
	//	{
	//		m_bth.afterPhysics[layer].pList[
	//			m_bth.afterPhysics[layer].num
	//		] = &m_pBone[i];
	//			m_bth.afterPhysics[layer].num++;
	//	}
	//	else
	//	{
	//		m_bth.prePhysics[layer].pList[
	//			m_bth.prePhysics[layer].num
	//		] = &m_pBone[i];
	//			m_bth.prePhysics[layer].num++;
	//	}
	//}
	// 終了
	delete[] pIdxWork;
	return true;
}


void MMDModel::Draw()
{
	int start = 0;
	for (int i = 0; i < m_materialNum; ++i)
	{
		int tex = m_pMaterial[i].texture;
		//ShaderBuffer::GetInstance().SetTexturePS(m_ppTexture[tex]);
		m_pBuffer->Draw1(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			start, m_pMaterial[i].faceNum);
		start += m_pMaterial[i].faceNum;
	}
}


void MMDModel::CalcBone()
{
	//DirectX::XMMATRIX mMat =
	//	DirectX::XMMatrixIdentity();
	//for (int i = 0; i < m_bth.layerNum; ++i)
	//{
	//	for (int j = 0;
	//		j < m_bth.prePhysics[i].num; ++j)
	//	{
	//		BoneInfo* pBone =
	//			m_bth.prePhysics[i].pList[j];
	//		if (pBone->parent == -1)
	//			mMat = DirectX::XMMatrixIdentity();
	//		else
	//			mMat = m_pBone[pBone->parent].mat;
	//		// 親の基準の相対座標を表す変換行列
	//		DirectX::XMMATRIX T =
	//			DirectX::XMMatrixTranslation(
	//				pBone->offset.x,
	//				pBone->offset.y,
	//				pBone->offset.z
	//			);
	//		DirectX::XMMATRIX rotX =
	//			DirectX::XMMatrixRotationX(
	//				DirectX::XMConvertToRadians(
	//					pBone->rotate.x));
	//		DirectX::XMMATRIX rotY =
	//			DirectX::XMMatrixRotationY(
	//				DirectX::XMConvertToRadians(
	//					pBone->rotate.y));
	//		DirectX::XMMATRIX rotZ =
	//			DirectX::XMMatrixRotationZ(
	//				DirectX::XMConvertToRadians(
	//					pBone->rotate.z));
	//		// 相対座標で変形後、親の変形を適用
	//		pBone->mat =
	//			rotX * rotY * rotZ * T * mMat;
	//	}
	//}
}
