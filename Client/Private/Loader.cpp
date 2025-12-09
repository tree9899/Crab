#include "Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Camera_Free.h"
#include "Mesh_Terrain.h"
#include "Player.h"
#include "Body.h"
#include "Weapon.h"
#include "Tree.h"

Loader::Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ GameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY ThreadMain(void* pArg)
{
	Loader* pLoader = static_cast<Loader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT Loader::Initialize(LEVEL eLoadingLevelID)
{
	m_eLoadingLevelID = eLoadingLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT Loader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT		hr = {};

	switch (m_eLoadingLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;
	case LEVEL::MAPTOOL:
		hr = Loading_For_MapTool();
		break;
	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT Loader::Loading_For_Logo()
{
	lstrcpy(m_szFPS, TEXT("텍스쳐를 로딩 중 입니다."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo2.jpg"), 1))))
		return E_FAIL;

	lstrcpy(m_szFPS, TEXT("객체원형을(를) 로딩 중 입니다."));
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		BackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT Loader::Loading_For_MapTool()
{
	lstrcpy(m_szFPS, TEXT("텍스쳐를 로딩 중 입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Texture_Terrain"),
		Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/sand3.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szFPS, TEXT("모델을(를) 로딩 중 입니다."));

	_matrix		PreTransformMatrix = XMMatrixIdentity();
	/* For.Prototype_Component_Model_GenericTerrainF */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_GenericTerrainF"),
		Model::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Terrain/Map1.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Crab */
	PreTransformMatrix =
		XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) *
		XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_Crab"),
		Model::Create(m_pDevice, m_pContext, MODEL::ANIM, "../Bin/Resources/Models/Crab/Crab2.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_ForkLift */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_ForkLift"),
		Model::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_TreeE */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_TreeE"),
		Model::Create(m_pDevice, m_pContext, MODEL::NONANIM, "../Bin/Resources/Models/Tree/TreeE7.fbx", PreTransformMatrix))))
		return E_FAIL;

#pragma region 맵툴씬_쉐이더
	lstrcpy(m_szFPS, TEXT("쉐이더을(를) 로딩 중 입니다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxNorTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxMesh"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxPosTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Shader_VtxMesh_Terrain"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh_Terrain.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szFPS, TEXT("네비게이션을(를) 로딩 중 입니다."));
	///* For.Prototype_Component_Navigation */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Navigation"),
	//	Navigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation1.dat"), TEXT("../Bin/DataFiles/NavigationNeighbors.dat")))))
	//	return E_FAIL;

#pragma region 맵툴씬_콜라이더
	lstrcpy(m_szFPS, TEXT("콜라이더을(를) 로딩 중 입니다."));
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_AABB"),
		Collider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_OBB"),
		Collider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_Component_Collider_Sphere"),
		Collider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szFPS, TEXT("객체원형을(를) 로딩 중 입니다."));
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_Free"),
		Camera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MeshTerrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MeshTerrain"),
		Mesh_Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Player"),
		Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Body_Player"),
		Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Weapon_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Weapon_Player"),
		Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_TO_UINT(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Tree"),
		Tree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szFPS, TEXT("로딩이 완료되었슴니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT Loader::Loading_For_GamePlay()
{
	return S_OK;
}

Loader* Loader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLoadingLevelID)
{
	Loader* pInstance = new Loader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eLoadingLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Loader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);


}