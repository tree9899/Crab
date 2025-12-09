#include "Layer.h"

#include "GameObject.h"

Layer::Layer()
{
}

GameObject* Layer::Get_GameObject(_uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_GameObjects.end())
		return nullptr;

	return *iter;	
}

HRESULT Layer::Add_GameObject(GameObject* pGameObject)
{
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void Layer::Update_Priority(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if(nullptr != pGameObject)
			pGameObject->Update_Priority(fTimeDelta);
	}
}

void Layer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update(fTimeDelta);
	}
}

void Layer::Update_Late(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update_Late(fTimeDelta);
	}
}

Layer* Layer::Create()
{
	return new Layer();
}

void Layer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
