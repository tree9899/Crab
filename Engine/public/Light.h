#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Light final : public Base
{
private:
	Light();
	virtual ~Light() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);

private:
	LIGHT_DESC				m_LightDesc{};

public:
	static Light* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

NS_END