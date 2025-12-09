#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL GameObject abstract : public Base
{
public:
	typedef struct tagGameObjectDesc : public Transform::TRANSFORM_DESC
	{
		/* 객체에게 공통적으로 필요한 요소 */
		_tchar	szName[MAX_PATH] = {};
	}GAMEOBJECT_DESC;
protected:
	GameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	GameObject(const GameObject& Prototype); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~GameObject() = default;

public:
	class Component* Get_Component(const _wstring& strComponentTag);

public:
	virtual HRESULT Initialize_Prototype(); /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg); /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Update_Late(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class GameInstance* m_pGameInstance = { nullptr };

protected:
	_tchar			  m_szName[MAX_PATH] = {};
	map<const _wstring, class Component*>		m_Components;
	class Transform* m_pTransformCom = { nullptr };

protected:
	HRESULT Add_Component(_uint iPrototypeLevelID, const _wstring& strPrototypeTag, 
		const _wstring& strComponentTag, Component** ppOut, void* pArg = nullptr);

public:	
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END