#pragma once
#include <memory>
#include <vector>
#include <Vector.h>

class Image;

class TexAnimation
{
public:
	TexAnimation() {}
	virtual ~TexAnimation() {}

	void LoadData(const char* pPath);
	void Reset(const int nType);
	void Update(const int nType);
	void Bind();

private:
	std::shared_ptr<Image> m_pImage;
	std::vector<Vector2> m_vStartOffsetList;
	float m_fAnimSpeed;
	float m_fTime;
	VectorInt2 m_vSheetsNum;
	VectorInt2 m_vSheet;
	VectorInt2 m_vStartSheet;
	int m_nAnimType;
};