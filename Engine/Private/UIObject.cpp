#include "UIObject.h"
#include "Shader.h"

UIObject::UIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : GameObject { pDevice, pContext }
{
}

UIObject::UIObject(const UIObject& Prototype)
    : GameObject{ Prototype }
{
}

HRESULT UIObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT UIObject::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_fX = pDesc->fX;
    m_fY = pDesc->fY;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    D3D11_VIEWPORT          ViewportDesc{};
    _uint           iNumViewports = { 1 };

    m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


    /* 직교투영을 위한 월드, 뷰, 투영행렬을 셋팅한다. */
    m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(
        m_fX - ViewportDesc.Width * 0.5f,
        -m_fY + ViewportDesc.Height * 0.5f,
        0.f, 1.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

    return S_OK;
}

void UIObject::Update_Priority(_float fTimeDelta)
{
}

void UIObject::Update(_float fTimeDelta)
{
}

void UIObject::Update_Late(_float fTimeDelta)
{
}

HRESULT UIObject::Render()
{
    return S_OK;
}

HRESULT UIObject::Bind_OrthoMatrices(Shader* pShader, const _char* pViewMatrixName, const _char* pProjMatrixName)
{
    /* 뷰, 투영행렬을 렌더링할 때 이용할 수 있도록 셋팅해주는 작업. */
    if (FAILED(pShader->Bind_Matrix(pViewMatrixName, &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(pShader->Bind_Matrix(pProjMatrixName, &m_ProjMatrix)))
        return E_FAIL;

    return S_OK;
}

void UIObject::Free()
{
    __super::Free();


}
