#pragma once

#include "Texture.h"
#include <vector>
#include <map>
#include <MyMath.h>
#include <iostream>
#include <fstream>
#include <sstream>

class NewOBJParser
{
private:
	struct Material
	{
		float Ns;
		Vector3 Ka;
		Vector3 Kd;
		Vector3 Ks;
		Vector3 Ke;
		float Ni;
		float d;
		UINT8 illum;
	};

public:
	NewOBJParser() {}
	~NewOBJParser() {}

	const bool Load(const std::string& path, const std::string& name);

private:
	const bool LoadMaterial(const std::string& str);

private:
	std::vector<int> m_vtxIndexList;
	std::vector<int> m_uvIndexList;
	std::vector<int> m_normalIndexList;
	
	std::map<std::string, Material> m_materialList;
};

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