#include "Prototype_Manager.h"

#include "GameObject.h"
// #include "Component.h"

Prototype_Manager::Prototype_Manager()
{
}

HRESULT Prototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototypes = new map<const _wstring, Base*>[iNumLevels];

	return S_OK;
}

HRESULT Prototype_Manager::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, Base* pPrototype)
{
	if (iLevelIndex >= m_iNumLevels ||
		nullptr != Find_Prototype(iLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iLevelIndex].emplace(strPrototypeTag, pPrototype);	

	return S_OK;
}

Base* Prototype_Manager::Clone_Prototype(PROTOTYPE ePrototypeID, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	Base*	pPrototype = Find_Prototype(iLevelIndex, strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	if (PROTOTYPE::GAMEOBJECT == ePrototypeID)
		return dynamic_cast<GameObject*>(pPrototype)->Clone(pArg);
	else
		return dynamic_cast<Component*>(pPrototype)->Clone(pArg);
}

void Prototype_Manager::Clear(_uint iClearLevelID)
{
	for (auto& Pair : m_pPrototypes[iClearLevelID])
		Safe_Release(Pair.second);

	m_pPrototypes[iClearLevelID].clear();
}

Base* Prototype_Manager::Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag)
{
	auto	iter = m_pPrototypes[iLevelIndex].find(strPrototypeTag);

	if(iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

Prototype_Manager* Prototype_Manager::Create(_uint iNumLevels)
{
	Prototype_Manager* pInstance = new Prototype_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : Prototype_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void Prototype_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pPrototypes[i])		
			Safe_Release(Pair.second);

		m_pPrototypes[i].clear();	
	}
	Safe_Delete_Array(m_pPrototypes);



}
