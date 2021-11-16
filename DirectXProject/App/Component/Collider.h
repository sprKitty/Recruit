#pragma once
#include "Component.h"
#include <Transform.h>

//#include <App/Boxel/Boxel.h>

#include <vector>
class Collider : public Component
{
public:
	Collider();
	~Collider() override;

	//void Start() override;
	void Init() override;
	void Uninit() override;
	void Update() override;
	inline DirectX::XMFLOAT4X4 GetWorld() { return m_World; }
	DirectX::XMMATRIX GetWorldMatrix() { return m_wMatrix; }
	inline void SetIsParentWorld(bool enable = true) { m_isParentWorld = enable; }
	inline void SetisOutSideWorld(bool disable = false) { m_isOutSideWorld = disable; }
	inline void EnableHit() { m_isHit = true; }
	inline void DisableHit() { m_isHit = false; }
	void SetHitPos(Vector3 pos) { m_vHitPos = pos; }

	inline bool GetIsHit() { return m_isHit; }
	Vector3 GetHitPos() { return m_vHitPos; }
	Transform m_EditCollider;
	DirectX::XMMATRIX m_Matrix;
	//Box_Type m_BoxType;
	bool m_isActive;
private:
	DirectX::XMFLOAT4X4 m_World;
	DirectX::XMMATRIX m_wMatrix;
	Vector3 m_vHitPos;
	bool m_isParentWorld;
	bool m_isOutSideWorld;
	bool m_isHit;

	DirectX::XMFLOAT4X4 ConvertWorld();
};