#pragma once

#include <System/DrawBuffer.h>
#include <App/MeshData.h>
#include <MyMath.h>

class OBJModel
{
public:
	OBJModel();
	~OBJModel();

	static const MeshData::Info Load(const char* pPath);
	bool Create(const char* fileName);
	void Draw();

	inline int GetVtxNum() { return m_nVtxNum; }
	inline MyMath::Vertex* GetVtxList() { return m_pVtxList; }
private:
	DrawBuffer* m_pBuffer;

	int m_nVtxNum;
	MyMath::Vertex* m_pVtxList;
};
