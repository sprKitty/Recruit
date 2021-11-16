#ifndef __DRAW_BUFFER_H__
#define __DRAW_BUFFER_H__

#include "DirectX.h"

class DrawBuffer
{
public:
	DrawBuffer();
	~DrawBuffer();

	HRESULT CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite = false);
	HRESULT CreateIndexBuffer(const void* pIdx, UINT size, UINT count);
	HRESULT CreateIndex(const long *pIdx, UINT count);
	void Draw(D3D11_PRIMITIVE_TOPOLOGY primitive);
	void Draw1(D3D11_PRIMITIVE_TOPOLOGY primitive, int start = 0, int count = 0);
	HRESULT Write(void* pVtx);

private:
	ID3D11Buffer* m_pVertexBuffer;
	UINT m_vtxSize;
	UINT m_vtxCount;

	ID3D11Buffer* m_pIndexBuffer;
	UINT m_idxSize;
	UINT m_idxCount;
};



#endif // __DRAW_BUFFER_H__