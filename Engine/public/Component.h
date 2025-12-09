#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Component abstract : public Base
{
protected:
	Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Component(const Component& Prototype);
	virtual ~Component() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class GameInstance*	m_pGameInstance = { nullptr };

	_bool					m_isClone = { false };

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;


};


NS_END