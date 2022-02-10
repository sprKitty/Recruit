#pragma once
#include "Renderer.h"
#include <App/Image.h>

class Camera;
class TexAnimation;
class Mesh;
class Transform;
class FadeBase;

class BillBoardRenderer : public Renderer
{
public:
	BillBoardRenderer() {}
	~BillBoardRenderer()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void Write(const std::weak_ptr<ShaderBuffer>& pBuf, const WriteType::kind type)override;
	void Draw(const std::weak_ptr<ShaderBuffer>& pBuf, const DrawType::kind type)override;

	void CalcFrustumState(const std::weak_ptr<ViewPoint>& pVP)override;

	inline void XaxisLock() 
	{ 
		m_isXaxisLock = true;
	}

	inline void YaxisLock() 
	{ 
		m_isYaxisLock = true; 
	}

	inline void ZaxisLock() 
	{
		m_isZaxisLock = true;
	}
		   
	inline void XaxisUnlock() 
	{ 
		m_isXaxisLock = false;
	}

	inline void YaxisUnlock() 
	{
		m_isYaxisLock = false;
	}

	inline void ZaxisUnlock() 
	{ 
		m_isZaxisLock = false; 
	}

	inline void SetCamera(const std::weak_ptr<Camera>& pCamera)
	{
		m_pCamera = pCamera;
	}

	inline void SetImage(const std::string& str) { m_Image.SetTexture(str); }
	inline void SetMainTexAnimation(const std::weak_ptr<TexAnimation> pImage) { m_pMainTexAnim = std::move(pImage); }
	inline void SetBumpTexAnimation(const std::weak_ptr<TexAnimation> pImage) { m_pBumpTexAnim = std::move(pImage); }
	inline void SetFadeAnimation(const std::weak_ptr<FadeBase> pFade) { m_pFadeAnimation = std::move(pFade); }

	inline void EnableWrite(const WriteType::kind type) { m_isWriteType[type] = true; }
	inline void EnableDraw(const DrawType::kind type) { m_isDrawType[type] = true; }

	inline void DisableWrite(const WriteType::kind type) { m_isWriteType[type] = false; }
	inline void DisableDraw(const DrawType::kind type) { m_isDrawType[type] = false; }

private:
	void CalcBillBoard();

private:
	std::vector<bool> m_isWriteType;
	std::vector<bool> m_isDrawType;

	bool m_isXaxisLock;
	bool m_isYaxisLock;
	bool m_isZaxisLock;

	std::weak_ptr<Camera> m_pCamera;
	std::weak_ptr<Transform> m_pTransform;
	std::weak_ptr<TexAnimation> m_pMainTexAnim;
	std::weak_ptr<TexAnimation> m_pBumpTexAnim;
	std::weak_ptr<FadeBase> m_pFadeAnimation;
	std::weak_ptr<Mesh> m_pMesh;
	Image m_Image;
};