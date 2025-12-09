#include "GameObject.h"
#include "GameInstance.h"

#include "Transform.h"

GameObject::GameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }
    , m_pGameInstance{ GameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

GameObject::GameObject(const GameObject& Prototype)
    : m_pDevice { Prototype.m_pDevice }
    , m_pContext { Prototype.m_pContext }
    , m_pGameInstance{ GameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

Component* GameObject::Get_Component(const _wstring& strComponentTag)
{
    auto    iter = m_Components.find(strComponentTag);

    if(iter == m_Components.end())
        return nullptr;

    return iter->second;
}

HRESULT GameObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT GameObject::Initialize(void* pArg)
{
    m_pTransformCom = Transform::Create(m_pDevice, m_pContext);
    if (nullptr == m_pTransformCom)
        return E_FAIL;

    if (nullptr != pArg)
    {
        GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

        lstrcpy(m_szName, pDesc->szName);
    }

    

    /* 트랜스폼 컴포넌트를 생성한다. */
    /* 생성한 트랜스폼에게 pArg그대로 던져줄꺼야.  */


    if (FAILED(m_pTransformCom->Initialize(pArg)))
        return E_FAIL;

    m_Components.emplace(g_strTransformTag, m_pTransformCom);

    Safe_AddRef(m_pTransformCom);

    return S_OK;
}

void GameObject::Update_Priority(_float fTimeDelta)
{
}

void GameObject::Update(_float fTimeDelta)
{
}

void GameObject::Update_Late(_float fTimeDelta)
{
}

HRESULT GameObject::Render()
{
    return S_OK;
}


HRESULT GameObject::Add_Component(_uint iPrototypeLevelID, const _wstring& strPrototypeTag, const _wstring& strComponentTag, Component** ppOut, void* pArg)
{
    auto    iter = m_Components.find(strComponentTag);
    if (iter != m_Components.end())
        return E_FAIL;

    Component* pComponent = dynamic_cast<Component*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::COMPONENT, iPrototypeLevelID, strPrototypeTag, pArg));
    if (nullptr == pComponent)
        return E_FAIL;

    m_Components.emplace(strComponentTag, pComponent);

    *ppOut = pComponent;

    Safe_AddRef(pComponent);

    return S_OK;
}

void GameObject::Free()
{
    __super::Free();

    for (auto& Pair : m_Components)
        Safe_Release(Pair.second);
    m_Components.clear();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
