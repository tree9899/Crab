#include "Component.h"
#include "GameInstance.h"

Component::Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }
    , m_pGameInstance { GameInstance::GetInstance() }
    , m_isClone { false }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

Component::Component(const Component& Prototype)
    : m_pDevice { Prototype.m_pDevice }
    , m_pContext { Prototype.m_pContext }
    , m_pGameInstance{ GameInstance::GetInstance() }
    , m_isClone{ true }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT Component::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Component::Initialize(void* pArg)
{
    return S_OK;
}


void Component::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
