#pragma once
#include <MyMath.h>
#include <System/DrawBuffer.h>

class MMDModel
{
public:
	struct VertexInfo
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
	};
	struct MaterialInfo
	{
		int texture;
		int faceNum;
	};
	//struct BoneInfo
	//{
	//	int parent;
	//	// �e�̈ʒu�����Ƃ����ړ��Ɖ�]�f�[�^
	//	DirectX::XMFLOAT3 offset;
	//	DirectX::XMFLOAT3 rotate;
	//	// �e�̕ό`���q�ɓ`���\����
	//	// �s��̊|���Z���d�˂邱�ƂŎ���
	//	DirectX::XMMATRIX mat;
	//};
	// �{�[�����ёւ��p�̃f�[�^
	//struct BoneTransformHierarchy
	//{
	//	struct List
	//	{
	//		BoneInfo** pList;
	//		int num;
	//		List() : pList(nullptr), num(0) {}
	//	};
	//	List* prePhysics;
	//	List* afterPhysics;
	//	int layerNum;
	//};
public:
	MMDModel();
	~MMDModel();
	bool Create(const char* fileName);
	void Draw();
	void CalcBone();
private:
	DrawBuffer* m_pBuffer;
	VertexInfo* m_pVertex;
	int m_vtxNum;
	ID3D11ShaderResourceView** m_ppTexture;
	MaterialInfo* m_pMaterial;
	int m_materialNum;
	int m_boneNum;
	int m_nTexNum;
	//BoneInfo* m_pBone;
	//BoneTransformHierarchy m_bth;
};