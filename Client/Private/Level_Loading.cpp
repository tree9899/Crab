#include "Level_Loading.h"
#include "GameInstance.h"

#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "MapTool_Level.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 다음레벨에 대한 자원들을 준비한다.(추가스레드) */
	m_pLoader = Loader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	/* 자원을 준비하는 동안 로딩 장면을 렌더링 해준다.(메인스레드) */
	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished() &&
		GetKeyState(VK_RETURN) & 0x8000)
	{
		Level* pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL::LOGO:
			pNewLevel = Level_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL::MAPTOOL:
			pNewLevel = MapTool_Level::Create(m_pDevice, m_pContext);
			break;

		//case LEVEL::GAMEPLAY:
		//	pNewLevel = Level_GamePlay::Create(m_pDevice, m_pContext);
		//	break;
		}

		if (nullptr == pNewLevel)
			return;

		if (SUCCEEDED(m_pGameInstance->Change_Level(ENUM_TO_UINT(m_eNextLevelID), pNewLevel)))
			return;

		MSG_BOX("뭔가 좀 이상한데?");
	}
}

HRESULT Level_Loading::Render()
{
	m_pLoader->Output();

	return S_OK;
}

HRESULT Level_Loading::Ready_Layer_UI()
{
	return S_OK;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	Level_Loading* pInstance = new Level_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : Level_Loading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Level_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
