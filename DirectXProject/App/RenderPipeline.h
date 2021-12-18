#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Renderer/Renderer.h>
#include <Defines.h>


class MultiPass;
class Camera;
class Light;

namespace RenderingSetting
{
	enum RenderTarget_Type
	{
		RGBA32_,
		R32,

	};

};

class RenderPipeline : public Singleton<RenderPipeline>
{
public:
	friend class Singleton<RenderPipeline>;

private:
	using RendererPtrList = std::vector<std::weak_ptr<Renderer> >;

public:
	
	void Initialize()override;
	void Finalize()override;
	
	/*************************************************
	* @brief ���X�g�ɓo�^����Ă���I�u�W�F�N�g������
	* @param[Renderer3D_PTR&] pRenderer3D �����I�u�W�F�N�g�̃|�C���^
	*************************************************/	
	void ReleaseRenderer(const std::weak_ptr<Component>& pComponent);
	

	/*************************************************
	* @brief �����_�[�^�[�Q�b�g�̂ǂ��ɏ�����
	* @param[WriteType] typeW �������݃^�C�v
	*************************************************/
	void Write(const WriteType::kind& typeW);


	/*************************************************
	* @brief �ǂ������������łɕ`����
	* @param[DrawType] typeD �`�����^�C�v
	*************************************************/
	void Draw(const DrawType::kind& typeD);
	

	/*************************************************
	* @brief �����_�[�p�C�v���C���ɃI�u�W�F�N�g��ǉ�
	* @param[Renderer3D_PTR&] pRenderer3D �`���I�u�W�F�N�g
	*************************************************/
	void AddRenderer(const std::weak_ptr<Component>& pComponent);


	inline void SetCamera(const std::weak_ptr<Camera> pCam)
	{
		m_pRentCamera = pCam;
	}

	inline void SetLight(const std::weak_ptr<Light> pLight)
	{
		m_pRentLight = pLight;
	}

	ID3D11ShaderResourceView* GetRenderTex(const WriteType::kind type, const int num = 0);

protected:
	RenderPipeline() {}
	~RenderPipeline() override{}

private:

	void Call(WriteType::kind typeW, DrawType::kind typeD);

private:
	RendererPtrList m_pDrawList;
	std::vector<std::shared_ptr<MultiPass> > m_pMultiPassList;

	std::weak_ptr<Camera> m_pRentCamera;
	std::weak_ptr<Light> m_pRentLight;
};

