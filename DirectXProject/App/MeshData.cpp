#include "MeshData.h"
#include <App/OBJModel.h>
#include <System/Geometory.h>


void MeshData::Initialize()
{
	// プログラム内でポリゴンを作成
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

void MeshData::Load(std::string str)
{
	if (strstr(str.c_str(), ".obj"))
	{
		const std::string staticPath = "Assets/OBJ/";
		std::string path = str;
		size_t size = path.find(".");
		path.resize(size);
		m_ModelMap[path] = OBJModel::Load(staticPath, str);
		return;
	}
}

const MeshData::Info* MeshData::Get(std::string str)
{
	const size_t dot = str.find(".");
	if (dot != -1) //.objなどの拡張子がついていたとき削除
	{
		str.resize(dot);
	}

	const auto itr = m_ModelMap.find(str);
	if (itr != m_ModelMap.end())
	{
		return &itr->second;
	}
	return nullptr;
}