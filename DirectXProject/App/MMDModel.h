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
	//	// 親の位置を軸とした移動と回転データ
	//	DirectX::XMFLOAT3 offset;
	//	DirectX::XMFLOAT3 rotate;
	//	// 親の変形が子に伝わる表現は
	//	// 行列の掛け算を重ねることで実現
	//	DirectX::XMMATRIX mat;
	//};
	// ボーン並び替え用のデータ
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