#pragma once
#include "Renderer.h"
#include <App/MMDModel.h>
#include <App/OBJModel.h>
#include <System/DrawBuffer.h>


class Renderer3D : public Renderer
{
public:
	Renderer3D();
	~Renderer3D() override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	
	void Write(const WriteType::kind type)override;
	void Draw(const DrawType::kind type)override;

	void SetModel(const char* fileName);

	OBJModel* GetOBJModel() { return m_pOBJModel; }
	
	inline void SetBillBoard(bool flg) { m_isBillboard = flg; }
	inline void SetParentScale(bool flg) { m_isParentScale = flg; }

	inline void EnableWrite(const WriteType::kind type) { m_isWriteType[type] = true; }
	inline void EnableDraw(const DrawType::kind type) { m_isDrawType[type] = true; }
	
	inline void DisableWrite(const WriteType::kind type) { m_isWriteType[type] = false; }
	inline void DisableDraw(const DrawType::kind type) { m_isDrawType[type] = false; }

private:

	std::vector<bool> m_isWriteType;
	std::vector<bool> m_isDrawType;

	MMDModel* m_pMMDModel;
	OBJModel* m_pOBJModel;

	bool m_isBillboard;
	//êeÇÃscaleÇ™âeãøÇ∑ÇÈÇ©ÇµÇ»Ç¢Ç©
	bool m_isParentScale;
};