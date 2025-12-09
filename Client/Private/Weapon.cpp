#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"

Weapon::Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : PartObject{ pDevice, pContext }
{
}

Weapon::Weapon(const Weapon& Prototype)
    : PartObject{ Prototype }
{
}

HRESULT Weapon::Initialize_Prototype()
{
    
    return S_OK;
}

HRESULT Weapon::Initialize(void* pArg)
{
    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;    

    m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
    m_pTransformCom->Rotation(0.0f, XMConvertToRadians(90.0f), 0.f);
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.8f, 0.f, 0.f, 1.f));

    return S_OK;
}

void Weapon::Update_Priority(_float fTimeDelta)
{
    
}

void Weapon::Update(_float fTimeDelta)
{
    if (nullptr == m_pSocketMatrix || nullptr == m_pParentMatrix)
        return;

    _matrix     SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    for (size_t i = 0; i < 3; i++)
        SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);    

    _matrix     ParentMatrix = SocketMatrix * XMLoadFloat4x4(m_pParentMatrix);

    __super::SetUp_CombinedWorldMatrix(ParentMatrix);    

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

}

void Weapon::Update_Late(_float fTimeDelta)
{
    
    m_pGameInstance->Add_RenderObject(RENDERGROUP::NONBLEND, this);
}

HRESULT Weapon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;    

    _uint   iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
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


HRESULT Weapon::Ready_Components()
{
    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_ForkLift"),
        TEXT("Com_Model"), reinterpret_cast<Component**>(&m_pModelCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<Component**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Collider_OBB */
    Bounding_OBB::BOUNDING_OBB_DESC      OBBDesc{};
    OBBDesc.vAngles = _float3(0.0f, 0.0f, 0.0f);
    OBBDesc.vExtents = _float3(0.8f, 1.2f, 1.5f);
    OBBDesc.vCenter = _float3(0.f, OBBDesc.vExtents.y, 0.f);

    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider_OBB"), reinterpret_cast<Component**>(&m_pColliderCom), &OBBDesc)))
        return E_FAIL;


    return S_OK;
}

HRESULT Weapon::Bind_ShaderResources()
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

Weapon* Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Weapon* pInstance = new Weapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Weapon");
        Safe_Release(pInstance);
    }
    return pInstance;
}

GameObject* Weapon::Clone(void* pArg)
{
    Weapon* pInstance = new Weapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Weapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Weapon::Free() 
{
    __super::Free();


    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
