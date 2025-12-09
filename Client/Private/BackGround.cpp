#include "BackGround.h"
#include "GameInstance.h"
//이미지 띄워주는 직교투영 클래스??

BackGround::BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: UIObject{ pDevice, pContext }
{
}

BackGround::BackGround(const BackGround& Prototype)
	: UIObject{ Prototype }
{
}
HRESULT BackGround::Initialize_Prototype()
{
	return S_OK;
}
HRESULT BackGround::Initialize(void* pArg)
{
    UIObject::UIOBJECT_DESC        Desc{};

    Desc.fX = g_iWinSizeX >> 1;
    Desc.fY = g_iWinSizeY >> 1;
    Desc.fSizeX = g_iWinSizeX;
    Desc.fSizeY = g_iWinSizeY;
    lstrcpy(Desc.szName, TEXT("BackGround"));
    Desc.fSpeedPerSec = 10.f;
    Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}
void BackGround::Update_Priority(_float fTimeDelta)
{
    int a = 10;
}
void BackGround::Update(_float fTimeDelta)
{
    int a = 10;
}
void BackGround::Update_Late(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(RENDERGROUP::UI, this);
}
HRESULT BackGround::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}
HRESULT BackGround::Ready_Components()
{
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<Component**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<Component**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_TO_UINT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
        TEXT("Com_Texture"), reinterpret_cast<Component**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}
HRESULT BackGround::Bind_ShaderResources()
{
    /* 직교투영용 뷰, 투영행렬을 쉐이더에 전달할꺼야. */
    if (FAILED(__super::Bind_OrthoMatrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
        return E_FAIL;

    /* 직교투영용 월드행렬을 쉐이더에 전달한다.*/
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}

BackGround* BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    BackGround* pInstance = new BackGround(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : BackGround");
        Safe_Release(pInstance);
    }
    return pInstance;
}

GameObject* BackGround::Clone(void* pArg)
{
    BackGround* pInstance = new BackGround(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : BackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void BackGround::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
}