#include "Camera_Free.h"
#include "GameInstance.h"

Camera_Free::Camera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Camera{ pDevice, pContext }
{
}

Camera_Free::Camera_Free(const Camera_Free& Prototype)
    : Camera{ Prototype }
{
}

HRESULT Camera_Free::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Camera_Free::Initialize(void* pArg)
{
    CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

    m_fSensor = pDesc->fSensor;

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    return S_OK;
}

void Camera_Free::Update_Priority(_float fTimeDelta)
{
    if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
    {
        m_pTransformCom->Go_Straight(fTimeDelta);
    }

    if (GetKeyState('S') & 0x8000)
    {
        m_pTransformCom->Go_Backward(fTimeDelta);
    }

    if (GetKeyState('A') & 0x8000)
    {
        m_pTransformCom->Go_Left(fTimeDelta);
    }

    if (GetKeyState('D') & 0x8000)
    {
        m_pTransformCom->Go_Right(fTimeDelta);
    }

    //_long    MouseMove = {};

    //if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
    //{
    //    m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fSensor * MouseMove * fTimeDelta);
    //}

    //if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
    //{
    //    m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), m_fSensor * MouseMove * fTimeDelta);
    //}

    _ubyte rb = m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::RB);

    if (rb & 0x80)            // 오른쪽 버튼 누르고 있을 때만 카메라 회전
    {
        _long MouseMove = {};

        if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::X))
        {
            m_pTransformCom->Turn(
                XMVectorSet(0.f, 1.f, 0.f, 0.f),
                m_fSensor * MouseMove * fTimeDelta
            );
        }

        if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::Y))
        {
            m_pTransformCom->Turn(
                m_pTransformCom->Get_State(STATE::RIGHT),
                m_fSensor * MouseMove * fTimeDelta
            );
        }
    }

    __super::Update_PipeLine();
}

void Camera_Free::Update(_float fTimeDelta)
{

}

void Camera_Free::Update_Late(_float fTimeDelta)
{

}

HRESULT Camera_Free::Render()
{

    return S_OK;
}


Camera_Free* Camera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Camera_Free* pInstance = new Camera_Free(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : Camera_Free");
        Safe_Release(pInstance);
    }
    return pInstance;
}

GameObject* Camera_Free::Clone(void* pArg)
{
    Camera_Free* pInstance = new Camera_Free(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : Camera_Free");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Camera_Free::Free()
{
    __super::Free();

}