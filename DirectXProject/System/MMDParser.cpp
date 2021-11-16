#include <System/MMDParser.h>
#include <stdio.h>
#include <memory.h>
#include <Windows.h>


// --- マクロ ---
#define MMDREADVAL(val) \
fread(&val, sizeof(val), 1, fp)
#define MMDREADPTR(ptr, num) \
fread(ptr, sizeof(ptr[0]), num, fp)
#define MMDREADINDEX(val) \
switch(type) {\
case 1: MMDREADVAL(val._char); break; \
case 2: MMDREADVAL(val._short); break; \
case 4: MMDREADVAL(val._long); break; \
}
#define MMDREADTEXT(val) \
MMDReadText(fp, val, encode)
// @param[out] pBuf 書き込み先
// @param[in] encode 変換方式
void MMDReadText(
	FILE* fp, MMDParser::TextBuf& text, char encode)
{
	MMDREADVAL(text.len);
	text.pBuf = new char[text.len + 1];
	MMDREADPTR(text.pBuf, text.len);
	text.pBuf[text.len] = '\0';
	// マルチバイト文字セットなら終了
	if (encode == 1) { return; }
	wchar_t* pwBuf = new wchar_t[text.len / 2 + 1];
	memcpy(pwBuf, text.pBuf, text.len);
	pwBuf[text.len / 2] = L'\0';
	WideCharToMultiByte(CP_ACP, 0,
		pwBuf, -1, text.pBuf, text.len, NULL, NULL);
	delete[] pwBuf;
}


MMDParser::MMDParser()
	: m_vtxNum(0), m_pVtx(nullptr)
	, m_idxNum(0), m_pIdx(nullptr), m_idxSize(0)
	, m_textureNum(0), m_pTexture(nullptr)
	, m_materialNum(0), m_pMaterial(nullptr), m_matIdxSize(0)
	//, m_boneNum(0), m_pBone(nullptr), m_boneIdxSize(0)
{
}


MMDParser::~MMDParser()
{
	if (m_pVtx) { delete[] m_pVtx; }
	if (m_pIdx) { delete[] m_pIdx; }
	if (m_pTexture) { delete[] m_pTexture; }
	if (m_pMaterial) { delete[] m_pMaterial; }
	// Boneの解放処理
	//if (m_pBone)
	//{
	//	for (int i = 0; i < m_boneNum; ++i)
	//	{
	//		if (m_pBone[i].flg & 0x0020)
	//			delete[] m_pBone[i].ik.pLink;
	//	}
	//	delete[] m_pBone;
	//}
}


bool MMDParser::Load(const char * fileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp) { return false; }
	struct PMXHeader
	{
		unsigned char pmx[4];
		float version;
		unsigned char dataNum;
		unsigned char* pData;
	};
	PMXHeader pmxHeader;
	MMDREADPTR(pmxHeader.pmx, 4);
	MMDREADVAL(pmxHeader.version);
	MMDREADVAL(pmxHeader.dataNum);
	pmxHeader.pData =
		new unsigned char[pmxHeader.dataNum];
	MMDREADPTR(pmxHeader.pData, pmxHeader.dataNum);
	char type = pmxHeader.pData[5];
	char encode = pmxHeader.pData[0];
	// モデル情報
	struct ModelInfo
	{
		TextBuf modelName;
		TextBuf modelNameEN;
		TextBuf comment;
		TextBuf commentEN;
	};
	ModelInfo modelInfo;
	MMDREADTEXT(modelInfo.modelName);
	MMDREADTEXT(modelInfo.modelNameEN);
	MMDREADTEXT(modelInfo.comment);
	MMDREADTEXT(modelInfo.commentEN);
	//--- 頂点データ
	MMDREADVAL(m_vtxNum);
	m_pVtx = new VertexInfo[m_vtxNum];
	for (int i = 0; i < m_vtxNum; ++i)
	{
		MMDREADVAL(m_pVtx[i].m_pos);
		MMDREADVAL(m_pVtx[i].m_normal);
		MMDREADVAL(m_pVtx[i].m_uv);
		MMDREADVAL(m_pVtx[i].m_variation);
		switch (m_pVtx[i].m_variation)
		{
		case 0: // BDEF1
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef1.bone);
			break;
		case 1: // BDEF2
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef2.bone[0]);
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef2.bone[1]);
			MMDREADVAL(m_pVtx[i].m_variationDef.bdef2.weight);
			break;
		case 2: // BDEF4
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef4.bone[0]);
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef4.bone[1]);
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef4.bone[2]);
			MMDREADINDEX(m_pVtx[i].m_variationDef.bdef4.bone[3]);
			MMDREADPTR(m_pVtx[i].m_variationDef.bdef4.weight, 4);
			break;
		case 3: // SDEF
			MMDREADINDEX(m_pVtx[i].m_variationDef.sdef.bone[0]);
			MMDREADINDEX(m_pVtx[i].m_variationDef.sdef.bone[1]);
			MMDREADVAL(m_pVtx[i].m_variationDef.sdef.weight);
			MMDREADPTR(m_pVtx[i].m_variationDef.sdef.mat, 9);
			break;
		}
		MMDREADVAL(m_pVtx[i].m_edge);
	}
	// インデックスデータ
	MMDREADVAL(m_idxNum);
	type = pmxHeader.pData[2]; // 頂点インデックスサイズ
	m_idxSize = type;
	m_pIdx = new Index[m_idxNum];
	for (int i = 0; i < m_idxNum; ++i)
	{
		MMDREADINDEX(m_pIdx[i]);
	}
	// テクスチャデータ
	MMDREADVAL(m_textureNum);
	m_pTexture = new TextBuf[m_textureNum];
	for (int i = 0; i < m_textureNum; ++i)
	{
		MMDREADTEXT(m_pTexture[i]);
	}
	// マテリアルデータ
	type = pmxHeader.pData[4]; // 材質インデックスサイズ
	m_matIdxSize = type;
	MMDREADVAL(m_materialNum);
	m_pMaterial = new Material[m_materialNum];
	for (int i = 0; i < m_materialNum; ++i)
	{
		MMDREADTEXT(m_pMaterial[i].name);
		MMDREADTEXT(m_pMaterial[i].nameEN);
		MMDREADVAL(m_pMaterial[i].diffuse);
		MMDREADVAL(m_pMaterial[i].specular);
		MMDREADVAL(m_pMaterial[i].ambient);
		MMDREADVAL(m_pMaterial[i].bitFlag);
		MMDREADVAL(m_pMaterial[i].edgeColor);
		MMDREADVAL(m_pMaterial[i].edgeSize);
		MMDREADINDEX(m_pMaterial[i].texture);
		MMDREADINDEX(m_pMaterial[i].sphereTexture);
		MMDREADVAL(m_pMaterial[i].sphereMode);
		MMDREADVAL(m_pMaterial[i].toonFlg);
		if (m_pMaterial[i].toonFlg == 0)
		{
			MMDREADINDEX(m_pMaterial[i].toonTexture);
		}
		else
		{
			MMDREADVAL(m_pMaterial[i].toonTexture._char);
		}
		MMDREADTEXT(m_pMaterial[i].memo);
		MMDREADVAL(m_pMaterial[i].faceNum);
	}
	// ボーンの読み込み
	//m_boneIdxSize = pmxHeader.pData[5];
	//type = m_boneIdxSize;
	//MMDREADVAL(m_boneNum);
	//m_pBone = new Bone[m_boneNum];
	//for (int i = 0; i < m_boneNum; ++i)
	//{
	//	MMDREADTEXT(m_pBone[i].name);
	//	MMDREADTEXT(m_pBone[i].nameEN);
	//	MMDREADVAL(m_pBone[i].pos);
	//	MMDREADINDEX(m_pBone[i].parent);
	//	MMDREADVAL(m_pBone[i].hierarchy);
	//	MMDREADVAL(m_pBone[i].flg);
	//	if (m_pBone[i].flg & 0x0001)
	//	{
	//		MMDREADINDEX(m_pBone[i].child.index);
	//	}
	//	else
	//	{
	//		MMDREADVAL(m_pBone[i].child.pos);
	//	}
	//	if (m_pBone[i].flg & 0x0300)
	//	{
	//		MMDREADINDEX(m_pBone[i].append.parent);
	//		MMDREADVAL(m_pBone[i].append.rate);
	//	}
	//	if (m_pBone[i].flg & 0x0400)
	//	{
	//		MMDREADVAL(m_pBone[i].fixAxis);
	//	}
	//	if (m_pBone[i].flg & 0x0800)
	//	{
	//		MMDREADVAL(m_pBone[i].localAxis);
	//	}
	//	if (m_pBone[i].flg & 0x2000)
	//	{
	//		MMDREADVAL(m_pBone[i].pt);
	//	}
	//	if (m_pBone[i].flg & 0x0020)
	//	{
	//		MMDREADINDEX(m_pBone[i].ik.index);
	//		MMDREADVAL(m_pBone[i].ik.loop);
	//		MMDREADVAL(m_pBone[i].ik.limit);
	//		MMDREADVAL(m_pBone[i].ik.linkNum);
	//		m_pBone[i].ik.pLink =
	//			new Bone::IK::Link[m_pBone[i].ik.linkNum];
	//		for (int j = 0; j < m_pBone[i].ik.linkNum; ++j)
	//		{
	//			MMDREADINDEX(m_pBone[i].ik.pLink[j].index);
	//			MMDREADVAL(m_pBone[i].ik.pLink[j].limit);
	//			if (m_pBone[i].ik.pLink[j].limit == 1)
	//			{
	//				MMDREADVAL(m_pBone[i].ik.pLink[j].minAngle);
	//				MMDREADVAL(m_pBone[i].ik.pLink[j].maxAngle);
	//			}
	//		}
	//	}
	//}
	// 終了処理
	delete[] pmxHeader.pData;
	fclose(fp);
	return true;
}
