#include "Level_GamePlay.h"
#include "GameInstance.h"

//Level_GamePlay::Level_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
//	:Level{ pDevice, pContext }
//{
//}
//
//HRESULT Level_GamePlay::Initialize()
//{
//	if (FAILED(Ready_Lights()))
//		return E_FAIL;
//
//	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
//		return E_FAIL;
//
//	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
//	//	return E_FAIL;
//
//	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
//	//	return E_FAIL;
//
//	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//void Level_GamePlay::Update(_float fTimeDelta)
//{
//	//__super::Update(fTimeDelta);
//}
//
//HRESULT Level_GamePlay::Render()
//{
//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("¸ÊÅø ·¹º§."));
//#endif
//	return S_OK;
//}
//
//HRESULT Level_GamePlay::Ready_Lights()
//{
//	return S_OK;
//}
//
//HRESULT Level_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
//{
//	return S_OK;
//}