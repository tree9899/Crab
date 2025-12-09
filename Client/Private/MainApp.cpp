#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

USING(Client)

MainApp::MainApp()
	: m_pGameInstance{ GameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT MainApp::Initialize()
{
	/* 내 게임의 초기화작업을 수행 */
	ENGINE_DESC		EngineDesc{};
	EngineDesc.hInstance = g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.eWindowMode = WINMODE::WIN;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.iNumLevels = ENUM_TO_UINT(LEVEL::END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Gara()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_For_Static()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

	return S_OK;
}

void MainApp::Update(_float fTimeDelta)
{
	/* 내 게임의 반복적인 갱신  */
	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT MainApp::Render()
{
	/* 내 게임의 반복적인 렌더  */
	_float4		vClearColor = _float4(0.f, 0.f, 1.f, 1.f);

	m_pGameInstance->Draw_Begin(&vClearColor);

	m_pGameInstance->Draw();

	m_pGameInstance->Draw_End();

	return S_OK;
}

HRESULT MainApp::Ready_Gara()
{
#pragma region 네비(필요없숨)
	//_ulong			dwByte = {};
//HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
//if (0 == hFile)
//	return E_FAIL;

//_float3		vPoints[3] = {};

//CloseHandle(hFile);
#pragma endregion

	/* 텍스쳐를 생성해내고, 파일로 저장하고, 픽셀정보를 수정하고. */
	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC		TextureDesc{};
	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;

	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint* pInitialPixels = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pInitialPixels, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	/* a b g r */
	pInitialPixels[0] = D3DCOLOR_ARGB(255, 255, 255, 0);

	D3D11_SUBRESOURCE_DATA		InitialData{};
	InitialData.pSysMem = pInitialPixels;
	InitialData.SysMemPitch = sizeof(_uint) * TextureDesc.Width;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	/* 만들어진 텍스쳐 객체의 픽셀 정보를 수정한다. */
	/*m_pVB->Lock();
	m_pTexture->LockRect();*/

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubResource);

	_uint* pPixels = static_cast<_uint*>(SubResource.pData);

	for (size_t i = 0; i < 256; i++)
	{
		for (size_t j = 0; j < 256; j++)
		{
			_uint		iIndex = i * 256 + j;

			if (j < 128)
				pPixels[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
			else
				pPixels[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);
		}

	}
	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"))))
		return E_FAIL;

	Safe_Delete_Array(pInitialPixels);
	Safe_Release(pTexture2D);

	return S_OK;
}

HRESULT MainApp::Start_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(ENUM_TO_UINT(LEVEL::LOADING), Level_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT MainApp::Ready_Prototype_For_Static()
{
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		VIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	return S_OK;
}

MainApp* MainApp::Create()
{
	MainApp* pInstance = new MainApp();

	/* 메인앱을 완벽히 사용하기 위한 기타 초기화작업을 수행한다. */
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : MainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MainApp::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	/* 내 멤버를 정리한다. */
	Safe_Release(m_pGameInstance);
}
