#include "PartObject.h"

PartObject::PartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : GameObject { pDevice, pContext }
{

}

PartObject::PartObject(const PartObject& Prototype)
    : GameObject{ Prototype }
{

}

HRESULT PartObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT PartObject::Initialize(void* pArg)
{
    PARTOBJECT_DESC* pDesc = static_cast<PARTOBJECT_DESC*>(pArg);

    m_pParentMatrix = pDesc->pParentMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    return S_OK;
}

void PartObject::Update_Priority(_float fTimeDelta)
{
}

void PartObject::Update(_float fTimeDelta)
{
}

void PartObject::Update_Late(_float fTimeDelta)
{
}

HRESULT PartObject::Render()
{
    return S_OK;
}

void PartObject::SetUp_CombinedWorldMatrix(_fmatrix ParentMatrix)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()) * ParentMatrix);

}

void PartObject::Free()
{
    __super::Free();

}
