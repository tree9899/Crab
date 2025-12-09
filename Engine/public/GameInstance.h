#pragma once

/* 1. 여러 매니져 클래스를 들고 있다. */
/* 2. 여러 매니져 클래스의 함수를 호출해 준다. */
/* 3. 내 게임이 끝날때 알아서 매니져 정리한다. */
/* 4. 알아서 업데이트한다. */
/* 4. 알아서 렌더링한다. */
#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL GameInstance final : public Base
{
	DECLARE_SINGLETON(GameInstance)

private:
	GameInstance();
	virtual ~GameInstance() = default;

#pragma region Engine
public:	
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw_Begin(const _float4* pClearColor);
	HRESULT Draw();
	HRESULT Draw_End();
	HRESULT Clear(_uint iLevelID); /* 정해진레벨의 자원을 날린다. */
	_float Random(_float fMin, _float fMax);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);	
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion

	
		

#pragma region TIMER_MANAGER
public:
	_float			Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT			Add_Timer(const _tchar* pTimerTag);
	void			Compute_TimeDelta(const _tchar* pTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT Change_Level(_uint iCurrentLevelID, class Level* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, Base* pPrototype);
	Base* Clone_Prototype(PROTOTYPE ePrototypeID, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion


#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject_To_Layer(_uint iProtoLevelIndex, const _wstring& strPrototypeTag,
		_uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class GameObject* Find_GameObject(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);

#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderObject(RENDERGROUP eGroupID, class GameObject* pRenderObject);
#pragma endregion 

#pragma region PIPELINE
	const _float4x4* Get_Transform(D3DTS eTransformMatrix);
	const _float4* Get_CamPosition();
	void Set_Transform(D3DTS eTransformMatrix, _fmatrix TransformMatrix);
	HRESULT Bind_PipeLineMatrix(class Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix);
	HRESULT Bind_PipeLineInverseMatrix(class Shader* pShader, const _char* pConstant, D3DTS eTransformMatrix);
	HRESULT Bind_CamPosition(class Shader* pShader, const _char* pConstant);
#pragma endregion 

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
#pragma endregion 



private:
	class Graphic_Device*			m_pGraphic_Device = { nullptr };
	class Input_Device*			m_pInput_Device = { nullptr };
	class Level_Manager*			m_pLevel_Manager = { nullptr };
	class Prototype_Manager*		m_pPrototype_Manager = { nullptr };
	class Object_Manager*			m_pObject_Manager = { nullptr };
	class Timer_Manager*			m_pTimer_Manager = { nullptr };
	class Renderer*				m_pRenderer = { nullptr };
	class PipeLine*				m_pPipeLine = { nullptr };
	class Light_Manager*			m_pLight_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;

};

NS_END