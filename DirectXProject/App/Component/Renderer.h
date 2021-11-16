#pragma once
#include <App/Component/Component.h>
#include <App/MMDModel.h>
#include <App/FBXModel.h>
#include <App/OBJModel.h>
#include <System/DrawBuffer.h>

class Renderer : public Component
{
public:
	Renderer();
	~Renderer() override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetModel(const char* fileName);

	OBJModel* GetOBJModel() { return m_pOBJModel; }
	
	inline void SetBillBoard(bool flg) { m_isBillboard = flg; }
	inline void SetParentScale(bool flg) { m_isParentScale = flg; }

	void WriteShadow();
	void DrawShadow();
	void DrawWater();

private:
	//FBXModel* m_pFBXModel;
	MMDModel* m_pMMDModel;
	OBJModel* m_pOBJModel;
	bool m_isBillboard;
	//�e��scale���e�����邩���Ȃ���
	bool m_isParentScale;
	// �V���h�E�}�b�v�ɏ������ނ�
	bool m_isWriteShadow;
};