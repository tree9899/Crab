#include "Body.h"
#include "Player.h"
#include "GameInstance.h"

Body::Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : PartObject{ pDevice, pContext }
{
}

Body::Body(const Body& Prototype)
    : PartObject{ Prototype }
{
}

const _float4x4* Body::Get_SocketMatrix(const _char* pBoneName)
{
    return m_pModelCom->Get_BoneMatrixPtr(pBoneName);
}

HRESULT Body::Initialize_Prototype()
{
    
    return S_OK;
}

HRESULT Body::Initialize(void* pArg)
{
    BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->Set_Animation(3, true);

    return S_OK;
}

void Body::Update_Priority(_float fTimeDelta)
{

}

void Body::Update(_float fTimeDelta)
{
    if (nullptr == m_pParentMatrix)
        return;

    if (*m_pParentState & Player::IDLE)
    {
        m_pModelCom->Set_Animation(3, true);
    }

    if (*m_pParentState & Player::RUN)
    {
        m_pModelCom->Set_Animation(4, true);
    }


    m_pModelCom->Play_Animation(fTimeDelta);

    __super::SetUp_CombinedWorldMatrix(XMLoadFloat4x4(m_pParentMatrix));

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

}

void Body::Update_Late(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(RENDERGROUP::NONBLEND, this);
}

HRESULT Body::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;    

    _uint   iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
            return E_FAIL;
        /*m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0);*/

        if (FAILED(m_pModelCom->Bind_Bones(m_pShaderCom, "g_BoneMatrices", i)))
            return E_FAIL;
        
        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif

    return S_OK;
}


HRESULT Body::Ready_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_Crab"),
        TEXT("Com_Model"), reinterpret_cast<Component**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<Component**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Collider_Sphere */
    Bounding_Sphere::BOUNDING_SPHERE_DESC      SphereDesc{};
    SphereDesc.fRadius = 0.5f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_Collider_Sphere"), reinterpret_cast<Component**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;
    

    return S_OK;
}

HRESULT Body::Bind_ShaderResources()
{
    /*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;*/
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
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

    return S_OK;
}

Body* Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Body* pInstance = new Body(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Body");
        Safe_Release(pInstance);
    }
    return pInstance;
}

GameObject* Body::Clone(void* pArg)
{
    Body* pInstance = new Body(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Body");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Body::Free() 
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
