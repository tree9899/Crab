#include "Mesh_Terrain.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"

Mesh_Terrain::Mesh_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: GameObject{ pDevice, pContext }
{

}

Mesh_Terrain::Mesh_Terrain(const Mesh_Terrain& prototype)
	: GameObject{ prototype }
{

}

HRESULT Mesh_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Mesh_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))    // Transform 붙여주는 베이스 초기화
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void Mesh_Terrain::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void Mesh_Terrain::Update_Late(_float fTimeDelta)
{
	__super::Update_Late(fTimeDelta);

	// 렌더링 큐에 등록 (타일 터레인은 UI 그룹에 넣고 있었음 )
	m_pGameInstance->Add_RenderObject(RENDERGROUP::NONBLEND, this);
}

HRESULT Mesh_Terrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	const _uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		// 2) 셰이더 패스 시작 (0번 패스 사용)
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		// 3) 실제 메쉬 렌더
		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT Mesh_Terrain::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxMesh_Terrain"), 
		TEXT("Com_Shader"), reinterpret_cast<Component**>(&m_pShaderCom))))
		return E_FAIL;

	// 2) Model (Loader에서 등록한 통맵 모델 컴포넌트)
	if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_GenericTerrainF"), 
		TEXT("Com_Model"), reinterpret_cast<Component**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Texture_Terrain"),      // sand3.dds
		TEXT("Com_Texture"), reinterpret_cast<Component**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT Mesh_Terrain::Bind_ShaderResources()
{
		// 행렬
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_PipeLineMatrix(m_pShaderCom, "g_ViewMatrix", D3DTS::VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_PipeLineMatrix(m_pShaderCom, "g_ProjMatrix", D3DTS::PROJ)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_CamPosition(m_pShaderCom, "g_vCamPosition")))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	// Sand 텍스처(g_TerrainDiffuse) 바인딩
	if (FAILED(m_pTextureCom->Bind_ShaderResource(
		m_pShaderCom,
		"g_TerrainDiffuse",   // HLSL 변수 이름
		0)))                  // t0 슬롯 (Shader에서 register(t0))
		return E_FAIL;

	return S_OK;
}

Mesh_Terrain* Mesh_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Mesh_Terrain* pInstance = new Mesh_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Mesh_Terrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

GameObject* Mesh_Terrain::Clone(void* pArg)
{
	Mesh_Terrain* pInstance = new Mesh_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Mesh_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Mesh_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}