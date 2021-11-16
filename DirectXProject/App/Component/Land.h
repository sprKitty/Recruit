#pragma once
#include "Component.h"
#include <vector>
#include "System/DirectX.h"

class Land : public Component
{
public:
	struct Square
	{
		float max_x;
		float max_z;
		float min_x;
		float min_z;
		std::vector<Vector3> vtxList;
	};
public:
	Land();
	~Land();

	void Init() override;
	void Uninit() override;
	void Update() override;

	bool Collision(Vector3 vP0, Vector3 vW, XMFLOAT3* pX, XMFLOAT3* pN);
	static Land* m_pLand;
private:
	void CreateQuadTree();
	std::vector<Square*> m_pSquares;
	ID3D11ShaderResourceView* m_pTex;
	ID3D11ShaderResourceView* m_pTex1;
	ID3D11ShaderResourceView* m_pBump0;
	ID3D11ShaderResourceView* m_pBump1;
};