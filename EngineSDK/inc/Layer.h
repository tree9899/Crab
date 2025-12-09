#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Layer final : public Base
{
private:
	Layer();
	virtual ~Layer() = default;

public:
	class GameObject* Get_GameObject(_uint iIndex);

public:
	HRESULT Add_GameObject(class GameObject* pGameObject);
	void Update_Priority(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Update_Late(_float fTimeDelta);

private:
	list<class GameObject*>			m_GameObjects;

public:
	static Layer* Create();
	virtual void Free() override;
};

NS_END