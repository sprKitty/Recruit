#pragma once
#include <App/Component/Magic/MagicBase.h>
#include <App/TexAnimation.h>


class BillBoardRenderer;

class MagicRazer : public MagicBase
{
public:
	MagicRazer() {}
	~MagicRazer()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	inline void SetPos(const Vector3& vPos) { m_vPos = vPos; }
	inline void SetDirection(const Vector3& vDir) { m_vDirection = vDir; }
	inline void SetInitLength(const float fLength) { m_fInitLength = fLength; }
	inline void SetLength(const float fLength) { m_fLength = fLength; }

	inline const float GetLength() { return m_fLength; }

private:
	std::shared_ptr<TexAnimation> m_pTexAnim;
	Vector3 m_vDirection;
	Vector3 m_vPos;
	float m_fInitLength;
	float m_fLength;
};