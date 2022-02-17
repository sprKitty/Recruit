#pragma once

#include <System/DrawBuffer.h>
#include <App/MeshData.h>
#include <MyMath.h>

class OBJModel
{
private:
	struct VertexBuffer
	{
		MyMath::Vertex vtx;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		UINT16 specularIndex;
	};

public:
	OBJModel();
	~OBJModel();

	static const MeshData::Info Load(const std::string& path, const std::string& name);
	void Draw();

private:
	DrawBuffer* m_pBuffer;

};
