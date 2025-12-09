#pragma once

#include "Base.h"

NS_BEGIN(Engine)


class Timer_Manager : public Base
{
private:
	explicit Timer_Manager();
	~Timer_Manager() = default;

public:
	_float			Get_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT			Add_Timer(const _tchar* pTimerTag);
	void			Compute_TimeDelta(const _tchar* pTimerTag);
private:
	map<const _wstring, class Timer*>		m_mapTimer;	
private:
	class Timer* Find_Timer(const _tchar* pTimerTag);
public:
	static Timer_Manager* Create();
	virtual void		Free();
};

NS_END