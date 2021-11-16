#pragma once
#include <MyMath.h>

class MMDParser
{
public:
	struct TextBuf
	{
		int len;
		char* pBuf;
		TextBuf() : pBuf(NULL) {}
		~TextBuf() {
			if (pBuf)
			{
				delete[] pBuf;
				pBuf = NULL;
			}
		}
	};
	union Index
	{
		char _char;
		short _short;
		long _long;
	};
	struct BDEF1
	{
		Index bone;
	};
	struct BDEF2
	{
		Index bone[2];
		float weight;
	};
	struct BDEF4
	{
		Index bone[4];
		float weight[4];
	};
	struct SDEF
	{
		Index bone[2];
		float weight;
		float mat[9];
	};
	union VariationDef
	{
		BDEF1 bdef1;
		BDEF2 bdef2;
		BDEF4 bdef4;
		SDEF sdef;
	};
	struct VertexInfo
	{
		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_normal;
		DirectX::XMFLOAT2 m_uv;
		unsigned char m_variation;
		VariationDef m_variationDef;
		float m_edge;
	};
	struct Material
	{
		TextBuf name;
		TextBuf nameEN;
		DirectX::XMFLOAT4 diffuse; // 拡散光
		DirectX::XMFLOAT4 specular; // 反射光
		DirectX::XMFLOAT3 ambient; // 環境光
		unsigned char bitFlag;
		DirectX::XMFLOAT4 edgeColor;
		float edgeSize;
		Index texture;
		Index sphereTexture;
		unsigned char sphereMode;
		unsigned char toonFlg;
		Index toonTexture;
		TextBuf memo;
		int faceNum;
	};
	// 骨の情報
	struct Bone
	{
		union Child // 接続先
		{
			DirectX::XMFLOAT3 pos;
			Index index;
		};
		struct Append // 付与データ
		{
			Index parent;
			float rate;
		};
		struct FixAxis // 固定軸
		{
			DirectX::XMFLOAT3 axis;
		};
		struct LocalAxis // ローカル軸
		{
			DirectX::XMFLOAT3 xAxis;
			DirectX::XMFLOAT3 zAxis;
		};
		struct ParentTranform // 外部親変形
		{
			int key;
		};
		struct IK // IK(inverse kinematic
		{
			struct Link
			{
				Index index;
				unsigned char limit;
				DirectX::XMFLOAT3 minAngle;
				DirectX::XMFLOAT3 maxAngle;
			};
			Index index;
			int loop;
			float limit;
			int linkNum;
			Link* pLink;
		};
		TextBuf name;
		TextBuf nameEN;
		DirectX::XMFLOAT3 pos;
		Index parent;
		int hierarchy;
		short flg;
		Child child;
		Append append;
		FixAxis fixAxis;
		LocalAxis localAxis;
		ParentTranform pt;
		IK ik;
	};
public:
	MMDParser();
	~MMDParser();
	bool Load(const char* fileName);
	// 取得関数
	int GetVertexNum() { return m_vtxNum; }
	VertexInfo* GetVertex() { return m_pVtx; }
	int GetIndexNum() { return m_idxNum; }
	Index* GetIndex() { return m_pIdx; }
	char GetIndexSize() { return m_idxSize; }
	int GetTextureNum() { return m_textureNum; }
	TextBuf* GetTexture() { return m_pTexture; }
	int GetMaterialNum() { return m_materialNum; }
	Material* GetMaterial() { return m_pMaterial; }
	char GetMaterialIndexSize() { return m_matIdxSize; }
	//int GetBoneNum() { return m_boneNum; }
	//Bone* GetBone() { return m_pBone; }
	//char GetBoneIndexSize() { return m_boneIdxSize; }

private:
	int m_vtxNum; // 頂点数
	VertexInfo* m_pVtx; // 頂点情報
	int m_idxNum; // インデックス数
	Index* m_pIdx; // インデックスデータ
	char m_idxSize; // インデックスのバイト数
	int m_textureNum; // テクスチャ数
	TextBuf* m_pTexture;// テクスチャデータ
	int m_materialNum; // マテリアル数
	Material* m_pMaterial; // マテリアル
	char m_matIdxSize; // 材質インデックスのバイト数
	//int m_boneNum; // ボーン数
	//Bone* m_pBone; // ボーンデータ
	//char m_boneIdxSize; // ボーンインデックスのバイト数
};