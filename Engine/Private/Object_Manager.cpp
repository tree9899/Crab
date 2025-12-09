#include "Object_Manager.h"
#include "GameInstance.h"
#include "Layer.h"

#include "GameObject.h"

Object_Manager::Object_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { GameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);

}

HRESULT Object_Manager::Initialize(_uint iNumLevels)
{
	m_pLayers = new map<const _wstring, class Layer*>[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

void Object_Manager::Update_Priority(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update_Priority(fTimeDelta);		
	}
}

void Object_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void Object_Manager::Update_Late(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update_Late(fTimeDelta);
	}
}

void Object_Manager::Clear(_uint iClearLevelID)
{
	for (auto& Pair : m_pLayers[iClearLevelID])
		Safe_Release(Pair.second);

	m_pLayers[iClearLevelID].clear();
}

HRESULT Object_Manager::Add_GameObject_To_Layer(_uint iProtoLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	GameObject*		pGameObject = dynamic_cast<GameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iProtoLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	Layer*				pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = Layer::Create();
		pLayer->Add_GameObject(pGameObject);
		m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

GameObject* Object_Manager::Find_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	Layer*		pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_GameObject(iIndex);	
}


Layer* Object_Manager::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

Object_Manager* Object_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels)
{
	Object_Manager* pInstance = new Object_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : Object_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void Object_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);

		m_pLayers[i].clear();		
	}

	Safe_Delete_Array(m_pLayers);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
