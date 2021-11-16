#include "Geometory.h"
#include "DrawBuffer.h"
#include <MyMath.h>

//#define LINE_NUM (1000)
//#define LINE_GEOMETORY_NUM (LINE_NUM * 2)

//UINT g_lineIndex;
//GeometoryVertex g_lineData[LINE_GEOMETORY_NUM];
//DrawBuffer g_lines;
//	//for (int i = 0; i < _countof(vtxCube); ++i)
//	//{
//	//	vtxCube[i].color[0] = 0.1f;
//	//	vtxCube[i].color[1] = 0.8f;
//	//	vtxCube[i].color[2] = 0.1f;
//	//	vtxCube[i].color[3] = 1.0f;
//	//}
//	hr = g_cube.CreateVertexBuffer(vtxCube, sizeof(GeometoryVertex), _countof(vtxCube));
//	if (FAILED(hr)) { return hr; }
//void AddLine(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4 color)
//{
//	if (g_lineIndex < LINE_NUM)
//	{
//		g_lineData[g_lineIndex].pos[0] = start.x;
//		g_lineData[g_lineIndex].pos[1] = start.y;
//		g_lineData[g_lineIndex].pos[2] = start.z;
//		++g_lineIndex;
//		g_lineData[g_lineIndex].pos[0] = end.x;
//		g_lineData[g_lineIndex].pos[1] = end.y;
//		g_lineData[g_lineIndex].pos[2] = end.z;
//		++g_lineIndex;
//	}
//}
//void DrawLines()
//{
//	g_lines.Write(g_lineData);
//	g_lines.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	g_lineIndex = 0;
//}

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
