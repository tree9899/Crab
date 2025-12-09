#include "Renderer.h"
#include "GameObject.h"

Renderer::Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Renderer::Initialize()
{
	return S_OK;
}

HRESULT Renderer::Add_RenderObject(RENDERGROUP eGroupID, GameObject* pRenderObject)
{
	/*if (ENUM_TO_UINT(eGroupID) >= ENUM_TO_UINT(RENDERGROUP::END))*/
	if(eGroupID >= RENDERGROUP::END)
		return E_FAIL;

	m_RenderObjects[ENUM_TO_UINT(eGroupID)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

void Renderer::Draw()
{
	Render_Priority();
	Render_NonBlend();
	Render_Blend();
	Render_UI();
}

void Renderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::PRIORITY)])
	{
		if (nullptr != pRenderObject)		
			pRenderObject->Render();

		Safe_Release(pRenderObject);		
	}
	m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::PRIORITY)].clear();
}

void Renderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::NONBLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::NONBLEND)].clear();
}

void Renderer::Render_Blend()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::BLEND)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::BLEND)].clear();
}

void Renderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::UI)])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}
	m_RenderObjects[ENUM_TO_UINT(RENDERGROUP::UI)].clear();
}

Renderer* Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Renderer* pInstance = new Renderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Renderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void Renderer::Free()
{
	__super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)		
			Safe_Release(pRenderObject);
		RenderObjects.clear();		
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
