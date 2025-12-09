#include "MapTool_Level.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Free.h"

MapTool_Level::MapTool_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice, pContext }
{
}

HRESULT MapTool_Level::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Tree(TEXT("Layer_Tree"))))
		return E_FAIL;

	return S_OK;
}

void MapTool_Level::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_TO_UINT(LEVEL::LOADING), Level_Loading::Create(m_pDevice, m_pContext, LEVEL::GAMEPLAY))))
			return;
	}
}

HRESULT MapTool_Level::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("¸ÊÅø·¹º§."));
#endif
	return S_OK;
}

HRESULT MapTool_Level::Ready_Lights()
{
	LIGHT_DESC		LightDesc{};
	LightDesc.eType = LIGHT::DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT MapTool_Level::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	Camera_Free::CAMERA_FREE_DESC		CameraDesc{};
	CameraDesc.vPosition = _float3(0.f, 200.f, -20.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fSpeedPerSec = 100.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(180.0f);
	CameraDesc.fFovy = XMConvertToRadians(45.0f);
	CameraDesc.fNearZ = 0.1f;
	CameraDesc.fFarZ = 1000.f;
	CameraDesc.fSensor = 0.1f;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_TO_UINT(LEVEL::MAPTOOL), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT MapTool_Level::Ready_Layer_Player(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Player"),
		ENUM_TO_UINT(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

//HRESULT MapTool_Level::Ready_Layer_Monster(const _wstring& strLayerTag)
//{
//	//for (size_t i = 0; i < 1; i++)
//	//{
//	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster"),
//	//		ENUM_TO_UINT(LEVEL::GAMEPLAY), strLayerTag)))
//	//		return E_FAIL;
//	//}
//
//
//	return S_OK;
//}

HRESULT MapTool_Level::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MeshTerrain"),
		ENUM_TO_UINT(LEVEL::MAPTOOL), strLayerTag)))
		return E_FAIL;



	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(ENUM_TO_UINT(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
	//	ENUM_TO_UINT(LEVEL::GAMEPLAY), strLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT MapTool_Level::Ready_Layer_Tree(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < 20; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(
			ENUM_TO_UINT(LEVEL::MAPTOOL),
			TEXT("Prototype_GameObject_Tree"),
			ENUM_TO_UINT(LEVEL::MAPTOOL),
			strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

MapTool_Level* MapTool_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	MapTool_Level* pInstance = new MapTool_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : MapTool_Level");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MapTool_Level::Free()
{
	__super::Free();
}