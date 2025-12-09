#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class Timer final : public Base
{
private:
	Timer();
	virtual	~Timer() = default;

public:
	_float	Get_TimeDelta() const { return m_fTimeDelta; }

public:
	HRESULT			Ready_Timer();
	void			Update_Timer();

private:
	LARGE_INTEGER		m_FrameTime = {};
	LARGE_INTEGER		m_FixTime = {};
	LARGE_INTEGER		m_LastTime = {};
	LARGE_INTEGER		m_CpuTick = {};

	_float				m_fTimeDelta = {};

public:
	static		Timer* Create();
	virtual void	Free();
};

NS_END
