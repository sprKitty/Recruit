#pragma once
#include <App/Component/Component.h>
#include <System/ClassDesign/Property.h>
#include <System/DrawBuffer.h>
#include <MyMath.h>


class Transform;
class ShaderBuffer;

class Instancing : public Component
{
public:
	Instancing() 
	{
	}
	~Instancing()override {}
	void Init()override;
	void Uninit()override;
	void Update()override;
	
	void Bind(const std::weak_ptr<ShaderBuffer>& pBuf);
	void Set(const std::string str);

	void SetRandomXYZ(const Vector3& minLeft, const Vector3& maxRight, const VectorInt3& vNum, const Vector3& vMarginError);

public:
	Property<Vector3> scale;
	Property<Vector3> angle;

private:
	std::vector<Vector3> m_vPosList;
	std::weak_ptr<DrawBuffer> m_pDrawBuffer;
	PrimitiveType::Kind m_primitiveType;
};