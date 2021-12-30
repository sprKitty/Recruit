#pragma once
#include <memory>
#include <vector>
#include <Vector.h>

class Image;
class ShaderBuffer;

class TexAnimation
{
public:
	TexAnimation() {}
	virtual ~TexAnimation() {}

	void LoadData(const char* pPath);
	void Reset(const int nType);
	void Update(const int nType);
	void Bind(const std::weak_ptr<ShaderBuffer>& pBuf);

	const bool IsSheetUpdate(const VectorInt2& vSheet);

	inline const bool IsFinish()
	{
		return m_isFinish;
	}


private:
	std::shared_ptr<Image> m_pImage;
	std::vector<Vector2> m_vStartOffsetList;
	float m_fAnimSpeed;
	float m_fTime;
	VectorInt2 m_vSheetsNum;
	VectorInt2 m_vSheet;
	VectorInt2 m_vStartSheet;
	int m_nAnimType;
	int m_nNowAnim;
	bool m_isLoop;
	bool m_isFinish;
	bool m_isSheetUpdate;
};