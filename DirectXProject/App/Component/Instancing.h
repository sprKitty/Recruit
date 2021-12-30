#pragma once
#include <App/Component/Component.h>
#include <System/DrawBuffer.h>
#include <MyMath.h>


class Transform;
class ShaderBuffer;

class Instancing : public Component
{
public:
	Instancing() {}
	~Instancing()override {}
	void Init()override;
	void Uninit()override;
	void Update()override;
	
	void Bind(const std::weak_ptr<ShaderBuffer>& pBuf, const Vector3* vPos = nullptr);
	void Set(const std::string str);

	void SetRandomXYZ(const Vector3& minLeft, const Vector3& maxRight, const VectorInt3& vNum, const Vector3& vMarginError);

private:
	Vector3 m_vScale;
	Vector3 m_vAngle;
	std::vector<Vector3> m_vPosList;
	std::weak_ptr<DrawBuffer> m_pDrawBuffer;
	PrimitiveType::Kind m_primitiveType;
};