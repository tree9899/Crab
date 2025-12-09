#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL UIObject abstract : public GameObject
{
public:
	typedef struct tagUIObject final : public GameObject::GAMEOBJECT_DESC
	{
		_float		fX{}, fY{}, fSizeX{}, fSizeY{};
	}UIOBJECT_DESC;
protected:
	UIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UIObject(const UIObject& Prototype); /* 메모리 복사를 통해 객체를 생성. */
	virtual ~UIObject() = default;

public:
	virtual HRESULT Initialize_Prototype(); /* 원형객체가 만들어질 때 호출되는 함수. 무거운 초기화작업.  */
	virtual HRESULT Initialize(void* pArg); /* 원형을 복제하여 만든 사본객체가 생성시에 호출되는 함수. 추가적인 초기화를 위해 */
	virtual void Update_Priority(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Update_Late(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	/* 직교투영을 위한 데이터들. */
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

protected:
	HRESULT Bind_OrthoMatrices(class Shader* pShader, const _char* pViewMatrixName, const _char* pProjMatrixName);

public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END