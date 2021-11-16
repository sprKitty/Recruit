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
		DirectX::XMFLOAT4 diffuse; // �g�U��
		DirectX::XMFLOAT4 specular; // ���ˌ�
		DirectX::XMFLOAT3 ambient; // ����
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
	// ���̏��
	struct Bone
	{
		union Child // �ڑ���
		{
			DirectX::XMFLOAT3 pos;
			Index index;
		};
		struct Append // �t�^�f�[�^
		{
			Index parent;
			float rate;
		};
		struct FixAxis // �Œ莲
		{
			DirectX::XMFLOAT3 axis;
		};
		struct LocalAxis // ���[�J����
		{
			DirectX::XMFLOAT3 xAxis;
			DirectX::XMFLOAT3 zAxis;
		};
		struct ParentTranform // �O���e�ό`
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
	// �擾�֐�
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
	int m_vtxNum; // ���_��
	VertexInfo* m_pVtx; // ���_���
	int m_idxNum; // �C���f�b�N�X��
	Index* m_pIdx; // �C���f�b�N�X�f�[�^
	char m_idxSize; // �C���f�b�N�X�̃o�C�g��
	int m_textureNum; // �e�N�X�`����
	TextBuf* m_pTexture;// �e�N�X�`���f�[�^
	int m_materialNum; // �}�e���A����
	Material* m_pMaterial; // �}�e���A��
	char m_matIdxSize; // �ގ��C���f�b�N�X�̃o�C�g��
	//int m_boneNum; // �{�[����
	//Bone* m_pBone; // �{�[���f�[�^
	//char m_boneIdxSize; // �{�[���C���f�b�N�X�̃o�C�g��
};