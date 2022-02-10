#pragma once
#include <System/LoadFiles.h>
#include <System/Clocker.h>
#include <Shader/ShaderBuffer.h>
#include <System/ClassDesign/Property.h>

class FadeBase : public std::enable_shared_from_this<FadeBase>
{
public:
	FadeBase();
	virtual ~FadeBase();

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual const bool Update()final;
	virtual void Bind(const std::weak_ptr<ShaderBuffer> pSB) = 0;

	void StartFadeIn();
	void StartFadeOut();

public:
	Property<float> inOutSpeed;
	Property<float> time;

protected:
	void InUpdate();
	void OutUpdate();

protected:
	bool m_isIn;
	bool m_isOut;
	float m_fInOutSpeed;
	float m_fTime;
	float m_fInOut;
};