#ifndef __GEOMETORY_H__
#define __GEOMETORY_H__

#include <Windows.h>
#include <System/ClassDesign/Singleton.h>
#include <Vector.h>


class DrawBuffer;

class Geometory : public Singleton<Geometory>
{
public:
	friend class Singleton<Geometory>;

public:
	void Initialize()override {}
	void Finalize()override {}
	HRESULT Create();
	void DrawPolygon();
	void DrawCharacterPolygon();
	void DrawCube();

protected:
	Geometory() {}
	~Geometory()override {}

private:
	std::shared_ptr<DrawBuffer> m_pPolygon;
	std::shared_ptr<DrawBuffer> m_pCharacterPolygon;
	std::shared_ptr<DrawBuffer> m_pCube;
};

#endif // __GEOMETORY_H__