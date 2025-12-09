#include "GameInstance.h"

#include "Prototype_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Renderer.h"
#include "PipeLine.h"


IMPLEMENT_SINGLETON(GameInstance)

GameInstance::GameInstance()
{

}

#pragma region Engine
HRESULT GameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	/* 그래픽 디바이스 초기화 */
	m_pGraphic_Device = Graphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWindowMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = Input_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pLight_Manager = Light_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;


	m_pPipeLine = PipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;


	m_pLevel_Manager = Level_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pRenderer = Renderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* 인풋 디바이스 초기화 */
	/* 사운드  디바이스 초기화 */
	/* 타이머 매니져 초기화 */
	m_pTimer_Manager = Timer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return FALSE;

	/* 프로토타입매니져  초기화 */
	m_pPrototype_Manager = Prototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return FALSE;

	/* 오브젝트 매니져 초기화 */
	m_pObject_Manager = Object_Manager::Create(*ppDevice, *ppContext, EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return FALSE;

	return S_OK;
}
void GameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update();

	m_pObject_Manager->Update_Priority(fTimeDelta);

	m_pPipeLine->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Update_Late(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT GameInstance::Draw_Begin(const _float4* pClearColor)
{		
	if (FAILED(m_pGraphic_Device->Clear_BackBuffer_View(pClearColor)))
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Clear_DepthStencil_View()))
		return E_FAIL;

	return S_OK;
}
HRESULT GameInstance::Draw()
{
	m_pRenderer->Draw();

	m_pLevel_Manager->Render();

	return S_OK;
}
HRESULT GameInstance::Draw_End()
{
	m_pGraphic_Device->Present();

	return S_OK;
}
HRESULT GameInstance::Clear(_uint iLevelID)
{
	/* iLevelID용 자원을 삭제한다. */
	m_pObject_Manager->Clear(iLevelID);

	m_pPrototype_Manager->Clear(iLevelID);

	return S_OK;
}

_float GameInstance::Random(_float fMin, _float fMax)
{
	_float		fRandNormal = static_cast<_float>(rand()) / RAND_MAX;

	return (fMax - fMin) * fRandNormal + fMin;	
}

#pragma endregion

#pragma region INPUT_DEVICE 

_byte GameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}
_byte GameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}
_long GameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}
#pragma endregion

#pragma region TIMER_MANAGER
_float GameInstance::Get_TimeDelta(const _tchar* pTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT GameInstance::Add_Timer(const _tchar* pTimerTag)
{
	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

void GameInstance::Compute_TimeDelta(const _tchar* pTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(pTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT GameInstance::Change_Level(_uint iCurrentLevelID, Level* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iCurrentLevelID, pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT GameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, Base* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

Base* GameInstance::Clone_Prototype(PROTOTYPE ePrototypeID, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeID, iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT GameInstance::Add_GameObject_To_Layer(_uint iProtoLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject_To_Layer(iProtoLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);

}

GameObject* GameInstance::Find_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	return m_pObject_Manager->Find_GameObject(iLayerLevelIndex, strLayerTag, iIndex);
}

#pragma endregion

#pragma region RENDERER

HRESULT GameInstance::Add_RenderObject(RENDERGROUP eGroupID, GameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderObject(eGroupID, pRenderObject);
}
#pragma endregion

#pragma region PIPELINE
const _float4x4* GameInstance::Get_Transform(D3DTS eTransformMatrix)
{
	return m_pPipeLine->Get_Transform(eTransformMatrix);
}
const _float4* GameInstance::Get_CamPosition()
{
	return m_pPipeLine->Get_CamPosition();
}
void GameInstance::Set_Transform(D3DTS eTransformMatrix, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eTransformMatrix, TransformMatrix);
}

HRESULT GameInstance::Bind_PipeLineMatrix(Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix)
{
	return m_pPipeLine->Bind_PipeLineMatrix(pShader, pConstant, eTransformMatrix);
}

HRESULT GameInstance::Bind_PipeLineInverseMatrix(Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix)
{
	return m_pPipeLine->Bind_PipeLineInverseMatrix(pShader, pConstant, eTransformMatrix);
}

HRESULT GameInstance::Bind_CamPosition(Shader* pShader, const _char* pConstant)
{
	return m_pPipeLine->Bind_CamPosition(pShader, pConstant);
}

const LIGHT_DESC* GameInstance::Get_LightDesc(_uint iIndex)
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT GameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

#pragma endregion

void GameInstance::Release_Engine()
{
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);

	GameInstance::GetInstance()->DestroyInstance();
}

void GameInstance::Free()
{
	__super::Free();
	


}