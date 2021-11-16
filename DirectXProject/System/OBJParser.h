#pragma once

#include "Texture.h"
#include <vector>
#include <MyMath.h>

class OBJParser
{
public:
	OBJParser();
	~OBJParser();

	bool Load(const char*);

	std::vector<Vector3> GetVertexList() { return m_vtxList; }
	std::vector<Vector2> GetUVList() { return m_uvList; }
	std::vector<Vector3> GetNormalList() { return m_normalList; }
	std::vector<int> GetVertexIndexList() { return m_vtxIndexList; }
	std::vector<int> GetUVIndexList() { return m_uvIndexList; }
	std::vector<int> GetNormalIndexList() { return m_normalIndexList; }
private:
	std::vector<Vector3> m_vtxList;
	std::vector<Vector2> m_uvList;
	std::vector<Vector3> m_normalList;
	std::vector<int> m_vtxIndexList;
	std::vector<int> m_uvIndexList;
	std::vector<int> m_normalIndexList;
};