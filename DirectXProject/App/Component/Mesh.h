#pragma once
#include <App/Component/Component.h>
#include <MyMath.h>


class DrawBuffer;

namespace PrimitiveType
{
	enum Kind
	{
		TRIANGLELIST,
		TRIANGLESTRIP,

		MAX,
	};
}

class Mesh : public Component
{
public:
	struct Surface
	{
		Vector3 vPos0;
		Vector3 vPos1;
		Vector3 vPos2;
		Vector3 vNormal;
	};

	using SurfaceList = std::vector<Surface>;

public:
	Mesh() {}
	virtual ~Mesh() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Bind();

	void Set(const std::string str);

	inline const SurfaceList& GetSurfaceList()
	{
		return m_surfaceList;
	}

private:
	std::weak_ptr<DrawBuffer> m_pDrawBuffer;
	SurfaceList m_surfaceList;
	PrimitiveType::Kind m_primitiveType;
};