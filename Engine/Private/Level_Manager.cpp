#include "Level_Manager.h"
#include "Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

Level_Manager::Level_Manager()
	: m_pGameInstance { GameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance); 	
}

HRESULT Level_Manager::Change_Level(_uint iCurrentLevelID, Level* pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
		m_pGameInstance->Clear(m_iCurrentLevelID);

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;	

	m_iCurrentLevelID = iCurrentLevelID;

	return S_OK;
}


void Level_Manager::Update(_float fTimeDelta)
{
	m_pCurrentLevel->Update(fTimeDelta);

}

HRESULT Level_Manager::Render()
{
	return m_pCurrentLevel->Render();
}

Level_Manager* Level_Manager::Create()
{
	return new Level_Manager();
}

void Level_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}
