#pragma once
#include <System/ClassDesign/Singleton.h>
#include <App/Component/Renderer/Renderer.h>
#include <App/Camera.h>
#include <App/Light.h>
#include <Defines.h>


class MultiPass;
class ShaderBuffer;

class RenderPipeline : public Singleton<RenderPipeline>
{
public:
	friend class Singleton<RenderPipeline>;
	
	static const int KAWASE_BlOOM_NUM = 5;

	using KAWASE_BlOOM_VP = std::weak_ptr<ViewPoint>[KAWASE_BlOOM_NUM];
private:
	using RendererPtrList = std::vector<std::weak_ptr<Renderer> >;

public:
	
	void Initialize()override;
	void Finalize()override;
	
	/*************************************************
	* @brief リストに登録されているオブジェクトを消す
	* @param[Renderer3D_PTR&] pRenderer3D 消すオブジェクトのポインタ
	*************************************************/	
	void ReleaseRenderer(const std::weak_ptr<Component>& pComponent);
	
	void DrawShadowMap(const std::weak_ptr<Light> pLight);

	void DrawCameraDepth(const std::weak_ptr<Camera> pCamera);

	void PlayGaussianBlurX(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation = 2.0f, const int number = 0);
	
	void PlayGaussianBlurY(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVP, const float fDeviation = 2.0f, const int number = 0);

	void PlayKawaseBloom(const std::weak_ptr<Camera> pCamera, KAWASE_BlOOM_VP pVPList);

	void PlayMixTexture(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVPMix1, const std::weak_ptr<ViewPoint> pVPMix2);

	void DrawDOFTexture(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<ViewPoint> pVPDMain, const std::weak_ptr<ViewPoint> pVPDepth, const std::weak_ptr<ViewPoint> pVPBlur);

	void DrawWaterReflection(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP);

	void Draw(const std::weak_ptr<Camera> pCamera, const std::weak_ptr<Light> pLight, const std::weak_ptr<ViewPoint> pDOSVP, const std::weak_ptr<ViewPoint> pWaterRefVP);
	
	void DrawEffect(const std::weak_ptr<Camera> pCamera);

	/*************************************************
	* @brief レンダーパイプラインにオブジェクトを追加
	* @param[Renderer3D_PTR&] pRenderer3D 描くオブジェクト
	*************************************************/
	void AddRenderer(const std::weak_ptr<Component>& pComponent);

	inline void SetShaderBuffer(const std::weak_ptr<ShaderBuffer> ptr) { m_pShaderBuffer = ptr; }

protected:
	RenderPipeline() {}
	~RenderPipeline() override{}

private:
	void Call(WriteType::kind typeW, DrawType::kind typeD);

private:
	RendererPtrList m_pDrawList;
	std::weak_ptr<ShaderBuffer> m_pShaderBuffer;
};

