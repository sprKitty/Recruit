#include "Geometory.h"
#include "DrawBuffer.h"
#include <System/DebugLog.h>
#include <MyMath.h>


MeshData::Info Geometory::CreateBoardPolygon()
{
	HRESULT hr;
	MeshData::Info info;
	info.type = PrimitiveType::TRIANGLESTRIP;
	info.pDrawBuffer.reset(new DrawBuffer());

	const float d = 0.5f;
	MyMath::Vertex vtxPolygon[] =
	{
		{ Vector3(-d, d, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(d, d, 0), Vector2(1,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d,-d, 0), Vector2(0,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(d,-d, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
	};

	hr = info.pDrawBuffer->CreateVertexBuffer(vtxPolygon, sizeof(MyMath::Vertex), _countof(vtxPolygon));
	
	if (FAILED(hr))
	{
		DebugLog::GetInstance().FreeError("板ポリ生成失敗");
		info.pDrawBuffer.reset();
		info.surfaceList.clear();
	}
	return info;
}

MeshData::Info Geometory::CreateCharacterPolygon()
{
	HRESULT hr;
	MeshData::Info info;
	info.pDrawBuffer.reset(new DrawBuffer());

	const float d = 0.5f;
	MyMath::Vertex vtxCharacterPolygon[] =
	{
		{ Vector3(-d, d, 0), Vector2(0,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(d, d, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d,-d, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(d, d, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(d, -d, 0), Vector2(1,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d, -d, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
	};

	for (int i = 0; i < _countof(vtxCharacterPolygon); i += 3)
	{
		Vector3 vBio, vTan;
		Mesh::Surface surface;
		if (MyMath::TangentandBinormal(vtxCharacterPolygon[i], vtxCharacterPolygon[i + 1], vtxCharacterPolygon[i + 2], &vBio, &vTan))
		{
			vtxCharacterPolygon[i].binormal = vBio;
			vtxCharacterPolygon[i + 1].binormal = vBio;
			vtxCharacterPolygon[i + 2].binormal = vBio;
			vtxCharacterPolygon[i].tangent = vTan;
			vtxCharacterPolygon[i + 1].tangent = vTan;
			vtxCharacterPolygon[i + 2].tangent = vTan;
		}
		surface.vPos0 = vtxCharacterPolygon[i].pos;
		surface.vPos1 = vtxCharacterPolygon[i + 1].pos;
		surface.vPos2 = vtxCharacterPolygon[i + 2].pos;
		surface.vNormal = vtxCharacterPolygon[i].normal;
		info.surfaceList.push_back(surface);
	}

	hr = info.pDrawBuffer->CreateVertexBuffer(vtxCharacterPolygon, sizeof(MyMath::Vertex), _countof(vtxCharacterPolygon));

	if (FAILED(hr))
	{
		DebugLog::GetInstance().FreeError("キャラクター用ポリ生成失敗");
		info.pDrawBuffer.reset();
		info.surfaceList.clear();
	}
	return info;
}

MeshData::Info Geometory::CreateCube()
{
	HRESULT hr;
	MeshData::Info info;
	info.pDrawBuffer.reset(new DrawBuffer());

	const float d = 0.5f;

	MyMath::Vertex vtxCube[] =
	{
		{ {-d, d,-d}, {0,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {0,1}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,1}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {0,1}, {0,0,-1}, {0,0,0},{0,0,0} },

		{ { d, d,-d}, {0,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {0,1}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {1,1}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {0,1}, {1,0,0} , {0,0,0},{0,0,0} },

		{ { d, d, d}, {0,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d, d, d}, {1,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {0,1}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d, d, d}, {1,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {1,1}, {0,0,1} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {0,1}, {0,0,1} , {0,0,0},{0,0,0} },

		{ {-d, d, d}, {0,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {1,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {1,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {1,1}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {-1,0,0}, {0,0,0},{0,0,0} },

		{ {-d, d, d}, {0,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {0,1}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,1}, {0,1,0} , {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {0,1}, {0,1,0} , {0,0,0},{0,0,0} },

		{ {-d,-d,-d}, {0,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d, d}, {1,1}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {0,-1,0}, {0,0,0},{0,0,0} },
	};

	for (int i = 0; i < _countof(vtxCube); i += 3)
	{
		Vector3 vBio, vTan;
		Mesh::Surface surface;
		if (MyMath::TangentandBinormal(vtxCube[i], vtxCube[i + 1], vtxCube[i + 2], &vBio, &vTan))
		{
			vtxCube[i].binormal = vBio;
			vtxCube[i + 1].binormal = vBio;
			vtxCube[i + 2].binormal = vBio;
			vtxCube[i].tangent = vTan;
			vtxCube[i + 1].tangent = vTan;
			vtxCube[i + 2].tangent = vTan;
		}
		surface.vPos0 = vtxCube[i].pos;
		surface.vPos1 = vtxCube[i + 1].pos;
		surface.vPos2 = vtxCube[i + 2].pos;
		surface.vNormal = vtxCube[i].normal;
		info.surfaceList.push_back(surface);
	}

	hr = info.pDrawBuffer->CreateVertexBuffer(vtxCube, sizeof(MyMath::Vertex), _countof(vtxCube));

	if (FAILED(hr))
	{
		DebugLog::GetInstance().FreeError("キューブ生成失敗");
		info.pDrawBuffer.reset();
		info.surfaceList.clear();
	}
	return info;
}

HRESULT Geometory::Create()
{
	HRESULT hr;
	const float d = 0.5f;
	MyMath::Vertex vtxPolygon[] =
	{
		{ Vector3(-d, d, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3( d, d, 0), Vector2(1,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d,-d, 0), Vector2(0,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3( d,-d, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
	};
	m_pPolygon.reset(new DrawBuffer());
	hr = m_pPolygon->CreateVertexBuffer(vtxPolygon, sizeof(MyMath::Vertex), _countof(vtxPolygon));
	if (FAILED(hr)) return hr;
	
	MyMath::Vertex vtxCharacterPolygon[] =
	{
		{ Vector3(-d, d * 2.0f, 0), Vector2(0,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3( d, d * 2.0f, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d,        0, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3( d, d * 2.0f, 0), Vector2(1,0), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3( d,        0, 0), Vector2(1,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
		{ Vector3(-d,        0, 0), Vector2(0,1), Vector3(0,0,-1), Vector3(0), Vector3(0)},
	};
	for (int i = 0; i < _countof(vtxCharacterPolygon); i += 3)
	{
		Vector3 vBio, vTan;
		if (MyMath::TangentandBinormal(vtxCharacterPolygon[i], vtxCharacterPolygon[i + 1], vtxCharacterPolygon[i + 2], &vBio, &vTan))
		{
			vtxCharacterPolygon[i].binormal = vBio;
			vtxCharacterPolygon[i + 1].binormal = vBio;
			vtxCharacterPolygon[i + 2].binormal = vBio;
			vtxCharacterPolygon[i].tangent = vTan;
			vtxCharacterPolygon[i + 1].tangent = vTan;
			vtxCharacterPolygon[i + 2].tangent = vTan;
		}
	}
	m_pCharacterPolygon.reset(new DrawBuffer());
	hr = m_pCharacterPolygon->CreateVertexBuffer(vtxCharacterPolygon, sizeof(MyMath::Vertex), _countof(vtxCharacterPolygon));
	if (FAILED(hr)) return hr;

	MyMath::Vertex vtxCube[] =
	{
		{ {-d, d,-d}, {0,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {0,1}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,0}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,1}, {0,0,-1}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {0,1}, {0,0,-1}, {0,0,0},{0,0,0} },
		
		{ { d, d,-d}, {0,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {0,1}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {1,1}, {1,0,0} , {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {0,1}, {1,0,0} , {0,0,0},{0,0,0} },
		
		{ { d, d, d}, {0,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d, d, d}, {1,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {0,1}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d, d, d}, {1,0}, {0,0,1} , {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {1,1}, {0,0,1} , {0,0,0},{0,0,0} },
		{ { d,-d, d}, {0,1}, {0,0,1} , {0,0,0},{0,0,0} },
		
		{ {-d, d, d}, {0,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {1,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {1,0}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d,-d}, {1,1}, {-1,0,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {-1,0,0}, {0,0,0},{0,0,0} },
		
		{ {-d, d, d}, {0,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {0,1}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d, d}, {1,0}, {0,1,0} , {0,0,0},{0,0,0} },
		{ { d, d,-d}, {1,1}, {0,1,0} , {0,0,0},{0,0,0} },
		{ {-d, d,-d}, {0,1}, {0,1,0} , {0,0,0},{0,0,0} },
		
		{ {-d,-d,-d}, {0,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d,-d}, {1,0}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ { d,-d, d}, {1,1}, {0,-1,0}, {0,0,0},{0,0,0} },
		{ {-d,-d, d}, {0,1}, {0,-1,0}, {0,0,0},{0,0,0} },
	};
	for (int i = 0; i < _countof(vtxCube); i += 3)
	{
		Vector3 vBio, vTan;
		if (MyMath::TangentandBinormal(vtxCube[i], vtxCube[i + 1], vtxCube[i + 2], &vBio, &vTan))
		{
			vtxCube[i].binormal = vBio;
			vtxCube[i + 1].binormal = vBio;
			vtxCube[i + 2].binormal = vBio;
			vtxCube[i].tangent = vTan;
			vtxCube[i + 1].tangent = vTan;
			vtxCube[i + 2].tangent = vTan;
		}
	}
	m_pCube.reset(new DrawBuffer());
	hr = m_pCube->CreateVertexBuffer(vtxCube, sizeof(MyMath::Vertex), _countof(vtxCube));
	if (FAILED(hr)) return hr;

	return S_OK;
}

void Geometory::DrawPolygon()
{
	m_pPolygon->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Geometory::DrawCharacterPolygon()
{
	m_pCharacterPolygon->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Geometory::DrawCube()
{
	m_pCube->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
