#pragma once
#include <App/Component/Object.h>
#include <App/ViewPoint/Camera.h>
#include <App/ViewPoint/Light.h>
#include <System/RenderTarget.h>
#include <System/Input.h>
#include <System/ClassDesign/Delegate.h>
#include <App/Fade/FadeBase.h>
#include <Shader/ShaderBuffer.h>
#include <Defines.h>


namespace Scene_Type
{
	enum kind
	{
		TITLE = 0,
		GAME,

		MAX
	};
}

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase() 
	{
		std::unique_ptr<RenderTarget> pRT(new RenderTarget());
		pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
		//pRT->Create(SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		m_pMainScreen.reset(new Camera());
		m_pMainScreen->Init();
		m_pMainScreen->SetRenderTarget(pRT);
	}
	virtual ~SceneBase(){}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual const Scene_Type::kind Update() = 0;
	virtual void Draw() = 0;

	/*
	* @brief オブジェクトリストに追加
	*/
	void AddObject(const Object::OWNER_OBJ& pObj);
	
	/*
	* @brief ファクトリーのオブジェクトリストをシーンのリストに移動
	*/
	void MoveObject_FactoytoScene();

	/*
	* @brief オブジェクトリストからオブジェクトを削除
	*/
	void DeleteObject();

	const Object::WORKER_OBJECTLIST GetObjectList();

	inline void SetShaderBuffer(const std::weak_ptr<ShaderBuffer> ptr) { m_pShaderBuffer = ptr; }
	inline ID3D11ShaderResourceView* GetMainScreenTex() { return m_pMainScreen->GetRenderingTexture(0); }
	inline void SetFadeFunction(const std::shared_ptr<FadeBase>& pFade)
	{
		m_pFadeInFunction = Delegate<FadeBase, void>::CreateDelegator(pFade, &FadeBase::StartFadeIn);
		m_pFadeOutFunction = Delegate<FadeBase, void>::CreateDelegator(pFade, &FadeBase::StartFadeOut);
	}

protected:
	Object::OWNER_OBJECTLIST m_pObjList;
	std::list<Object*> m_pUIList;

	std::shared_ptr<Camera> m_pMainScreen;					// 最終的の描画情報を格納
	std::weak_ptr<ShaderBuffer> m_pShaderBuffer;			// 描画関連の設定
	std::shared_ptr<DelegateBase<void> > m_pFadeInFunction;	// フェードインを開始
	std::shared_ptr<DelegateBase<void> > m_pFadeOutFunction;	// フェードインを開始

private:
};