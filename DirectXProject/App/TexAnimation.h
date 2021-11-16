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
	void Reset();
	void Update(const int nType);
	void Bind();

private:
	std::shared_ptr<Image> m_pImage;
	std::vector<Vector2> m_vStartOffsetList;
	float m_fAnimSpeed;
	float m_fTime;
	int m_nSheetsX;
	int m_nSheetsY;
	int m_nSheet;
	int m_nAnimType;
};