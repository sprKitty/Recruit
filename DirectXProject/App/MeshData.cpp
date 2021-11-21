#include "MeshData.h"
#include <App/OBJModel.h>
#include <System/Geometory.h>


void MeshData::Initialize()
{
	std::string path;
	path = "board";
	m_ModelMap[path] = Geometory::CreateBoardPolygon();
	path = "character";
	m_ModelMap[path] = Geometory::CreateCharacterPolygon();
	path = "cube";
	m_ModelMap[path] = Geometory::CreateCube();
}

void MeshData::Finalize()
{
}

void MeshData::Load(const char * pName)
{
	if (strstr(pName, ".obj"))
	{
		const std::string staticPath = "Assets/OBJ/";
		std::string path = staticPath + pName;
		m_ModelMap[pName] = OBJModel::Load(path.c_str());
	}
}

const MeshData::Info& MeshData::Get(const std::string & str)
{
	for (const auto& itr : m_ModelMap)
	{
		if (str == itr.first)
		{
			return itr.second;
		}
	}
	return Info();
}