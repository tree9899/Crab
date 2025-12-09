#include "ContainerObject.h"
#include "GameInstance.h"
#include "PartObject.h"

ContainerObject::ContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : GameObject { pDevice, pContext }
{

}

ContainerObject::ContainerObject(const ContainerObject& Prototype)
    : GameObject{ Prototype }
{

}

HRESULT ContainerObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT ContainerObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void ContainerObject::Update_Priority(_float fTimeDelta)
{
    for (auto& Pair : m_PartObjects)
        Pair.second->Update_Priority(fTimeDelta);
}

void ContainerObject::Update(_float fTimeDelta)
{
    for (auto& Pair : m_PartObjects)
        Pair.second->Update(fTimeDelta);
}

void ContainerObject::Update_Late(_float fTimeDelta)
{
    for (auto& Pair : m_PartObjects)
        Pair.second->Update_Late(fTimeDelta);
}


HRESULT ContainerObject::Render()
{
    return S_OK;
}

HRESULT ContainerObject::Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartObjTag, void* pArg)
{
    if (nullptr != Find_PartObject(strPartObjTag))
        return E_FAIL;

    PartObject*        pPartObject = dynamic_cast<PartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pPartObject)
        return E_FAIL;

    m_PartObjects.emplace(strPartObjTag, pPartObject);

    return S_OK;
}

PartObject* ContainerObject::Find_PartObject(const _wstring& strPartTag)
{
    auto    iter = m_PartObjects.find(strPartTag);
    if(iter == m_PartObjects.end())
        return nullptr;

    return iter->second;
}

void ContainerObject::Free()
{
    __super::Free();

    for (auto& Pair : m_PartObjects)
        Safe_Release(Pair.second);

    m_PartObjects.clear();

}
