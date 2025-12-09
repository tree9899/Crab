#include "Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"

Level_Logo::Level_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Level{ pDevice, pContext }
{
}

HRESULT Level_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void Level_Logo::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_TO_UINT(LEVEL::LOADING), Level_Loading::Create(m_pDevice, m_pContext, LEVEL::MAPTOOL))))
			return;
	}

}

HRESULT Level_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT Level_Logo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	UIObject::UIOBJECT_DESC        Desc{};

	Desc.fX = {};
	Desc.fY = {};
	Desc.fSizeX = {};
	Desc.fSizeY = {};
	lstrcpy(Desc.szName, TEXT("BackGround"));
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = 0.f;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		ENUM_TO_UINT(LEVEL::LOGO), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

Level_Logo* Level_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Level_Logo* pInstance = new Level_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Level_Logo");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void Level_Logo::Free()
{
	__super::Free();

}