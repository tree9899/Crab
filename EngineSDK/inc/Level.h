#pragma once

#include "Base.h"

/* 클라이언트 개발자가 만드는 모들 레벨들의 부모가될 클래스다. */

NS_BEGIN(Engine)

class ENGINE_DLL Level abstract : public Base
{
protected:
	Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	class GameInstance*	m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free() override;
};

NS_END