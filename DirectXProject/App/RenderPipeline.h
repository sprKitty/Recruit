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
	* @brief リストに登録されているオブジェクトを消す
	* @param[Renderer3D_PTR&] pRenderer3D 消すオブジェクトのポインタ
	*************************************************/	
	void ReleaseRenderer(const std::weak_ptr<Component>& pComponent);
	

	/*************************************************
	* @brief レンダーターゲットのどこに書くか
	* @param[WriteType] typeW 書き込みタイプ
	*************************************************/
	void Write(const WriteType::kind& typeW);


	/*************************************************
	* @brief どういった感じでに描くか
	* @param[DrawType] typeD 描き方タイプ
	*************************************************/
	void Draw(const DrawType::kind& typeD);
	

	/*************************************************
	* @brief レンダーパイプラインにオブジェクトを追加
	* @param[Renderer3D_PTR&] pRenderer3D 描くオブジェクト
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

