#pragma once
#include "Renderer.h"


class Camera;
class Image;

class BillBoardRenderer : public Renderer
{
public:
	BillBoardRenderer() {}
	~BillBoardRenderer()override {}

	void Init()override;
	void Uninit()override;
	void Update()override;

	void Write(const WriteType::kind type)override;
	void Draw(const DrawType::kind type)override;

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

	inline void SetMainImage(const std::weak_ptr<Image> pImage)
	{
		m_pMainImage = std::move(pImage);
	}

	void SetBumpImage(const std::weak_ptr<Image> pImage)
	{
		m_pBumpImage = std::move(pImage);
	}

	inline void EnableWrite(const WriteType::kind type) { m_isWriteType[type] = true; }
	inline void EnableDraw(const DrawType::kind type) { m_isDrawType[type] = true; }

	inline void DisableWrite(const WriteType::kind type) { m_isWriteType[type] = false; }
	inline void DisableDraw(const DrawType::kind type) { m_isDrawType[type] = false; }

private:
	void CalcBillBoard(DirectX::XMMATRIX& mtx);

private:
	std::vector<bool> m_isWriteType;
	std::vector<bool> m_isDrawType;

	bool m_isXaxisLock;
	bool m_isYaxisLock;
	bool m_isZaxisLock;

	std::weak_ptr<Camera> m_pCamera;
	std::weak_ptr<Image> m_pMainImage;
	std::weak_ptr<Image> m_pBumpImage;
};