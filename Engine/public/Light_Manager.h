#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Light_Manager final : public Base
{
private:
	Light_Manager();
	virtual ~Light_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex);


public:
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);

private:
	list<class Light*>			m_Lights;

public:
	static Light_Manager* Create();
	virtual void Free() override;
};

NS_END