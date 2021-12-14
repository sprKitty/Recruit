#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Mesh.h>
#include <Vector.h>
#include <map>


class MeshData : public Singleton<MeshData>
{

public:
	friend class Singleton<MeshData>;

	struct Info
	{
		std::shared_ptr<DrawBuffer> pDrawBuffer;
		Mesh::SurfaceList surfaceList;
		PrimitiveType::Kind type = PrimitiveType::TRIANGLELIST;
	};

public:
	void Initialize()override;
	void Finalize()override;

	void Load(std::string str);
	const Info* Get(std::string str);

protected:
	MeshData() {}
	virtual ~MeshData() {}

private:
	std::map<std::string, Info> m_ModelMap;
};
