#include <App/Component/Renderer.h>
#include <App/Component/Object.h>

#include <App/RenderPipeline.h>
#include <App/Camera.h>
#include <Shader/ShaderBuffer.h>

#include <System/Input.h>
#include <System/Geometory.h>


Renderer::Renderer()
	//:m_pFBXModel(nullptr)
	: m_pMMDModel(nullptr)
	, m_pOBJModel(nullptr)
	, m_isWriteShadow(false)
	, m_isBillboard(false)
	, m_isParentScale(true)
{
}


Renderer::~Renderer()
{
}

void Renderer::Init()
{
	Uninit();
	RENDERPIPELINE->AddWriteList(RENDERPIPELINE->WRITE_DEPTH_OF_TERRAIN, this);
	RENDERPIPELINE->AddDrawList(RENDERPIPELINE->DRAW_WORLD_OF_TOON, this);
}

void Renderer::Uninit()
{
	if (m_pOBJModel)
	{
		delete m_pOBJModel;
		m_pOBJModel = nullptr;
	}
	if (m_pMMDModel)
	{
		delete m_pMMDModel;
		m_pMMDModel = nullptr;
	}
	//if (m_pFBXModel)
	//{
	//	delete m_pFBXModel;
	//	m_pFBXModel = nullptr;
	//}
}


void Renderer::Update()
{
}


void Renderer::Draw()
{
	SHADER->SetColor(XMFLOAT4(1, 1, 1, 1));
	Transform t = m_pOwner->GetTransform();
	XMMATRIX mtx, mtxParent;
	if (!m_isBillboard)
	{
		if (m_pOwner->GetParent())
		{
			if (m_isParentScale)
				mtxParent = m_pOwner->GetParent()->GetWorldMatrix();
			else
				mtxParent = m_pOwner->GetParent()->GetWorld();
			if (m_pOwner->GetParent()->GetParent())
			{
				if (m_isParentScale)
					mtxParent *= m_pOwner->GetParent()->GetParent()->GetWorldMatrix();
				else
					mtxParent *= m_pOwner->GetParent()->GetParent()->GetWorld();
			}
			mtx = MyMath::ConvertMatrix(t.scale, t.rot, t.pos);
			mtx *= mtxParent;
			SHADER->SetWorld(mtx);
		}
		else
			SHADER->SetWorld(m_pOwner->GetWorldMatrix());
	}
	else
	{
		XMMATRIX camM = Camera::m_pMain->GetView();
		XMMATRIX invCamM = XMMatrixInverse(nullptr, camM);
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, invCamM);
		mat._41 = 0;
		mat._42 = 0;
		mat._43 = 0;
		//mat._11 = 1;
		//mat._12 = 0;
		//mat._13 = 0;
		//mat._21 = 0;
		//mat._22 = 1;
		//mat._23 = 0;
		mat._31 = 0;
		mat._32 = 0;
		mat._33 = 1;
		invCamM = XMLoadFloat4x4(&mat);
		if (m_pOwner->GetParent())
		{
			Transform tP = m_pOwner->GetParent()->GetTransform();
			mtxParent = MyMath::ConvertMatrix(1, 0, tP.pos);
			mtx = XMMATRIX(
				XMMatrixScaling(t.scale.x, t.scale.y, 0)*
				invCamM*
				XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
			mtx *= mtxParent;
			SHADER->SetWorld(mtx);
		}
		else
		{
			mtx = XMMATRIX(
				XMMatrixScaling(t.scale.x, t.scale.y, 1)*
				invCamM*
				XMMatrixTranslation(t.pos.x, t.pos.y, t.pos.z));
			SHADER->SetWorld(mtx);
		}

	}
	if (m_pMMDModel)
	{
		m_pMMDModel->Draw();
		return;
	}
	//else if (m_pFBXModel)
	//{
	//	m_pFBXModel->Draw();
	//	return;
	//}
	else if (m_pOBJModel)
	{
		m_pOBJModel->Draw();
		return;
	}
	DrawCube();
}


void Renderer::SetModel(const char * fileName)
{
	HRESULT hr;
	if (strstr(fileName, ".pmx")/* && !m_pFBXModel*/ && !m_pOBJModel)
	{
		m_pMMDModel = new MMDModel();
		m_pMMDModel->Create(fileName);
		return;
	}
	//if (strstr(fileName, ".fbx") && !m_pMMDModel && !m_pOBJModel)
	//{
	//	m_pFBXModel = new FBXModel();
	//	m_pFBXModel->Load(fileName);
	//	return;
	//}
	if (strstr(fileName, ".obj")/* && !m_pFBXModel*/ && !m_pMMDModel)
	{
		m_pOBJModel = new OBJModel();
		m_pOBJModel->Create(fileName);
	}
}

void Renderer::WriteShadow()
{
	RENDERPIPELINE->AddWriteList(RenderPipeline::WRITE_DEPTH_OF_NEARSHADOW, this);
	RENDERPIPELINE->AddWriteList(RenderPipeline::WRITE_DEPTH_OF_MEDIUMSHADOW, this);
	RENDERPIPELINE->AddWriteList(RenderPipeline::WRITE_DEPTH_OF_FARSHADOW, this);
}

void Renderer::DrawShadow()
{
	RENDERPIPELINE->ReleaseDrawList(RenderPipeline::DRAW_WORLD_OF_TOON,this);
	RENDERPIPELINE->AddDrawList(RenderPipeline::DRAW_WORLD_OF_SHADOW, this);
}

void Renderer::DrawWater()
{
	RENDERPIPELINE->ReleaseWriteList(RenderPipeline::WRITE_DEPTH_OF_TERRAIN, this);
	RENDERPIPELINE->ReleaseDrawList(RenderPipeline::DRAW_WORLD_OF_TOON, this);
	RENDERPIPELINE->AddDrawList(RenderPipeline::DRAW_WORLD_OF_WATER, this);
}
