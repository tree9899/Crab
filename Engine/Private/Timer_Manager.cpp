#include "Timer_Manager.h"
#include "Timer.h"

Timer_Manager::Timer_Manager()
{
}

_float Timer_Manager::Get_TimeDelta(const _tchar* pTimerTag)
{
	Timer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

HRESULT Timer_Manager::Add_Timer(const _tchar* pTimerTag)
{
	Timer* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = Timer::Create();
	if (nullptr == pTimer)
		return E_FAIL;	

	m_mapTimer.emplace(pTimerTag, pTimer);

	return S_OK;
}

void Timer_Manager::Compute_TimeDelta(const _tchar* pTimerTag)
{
	Timer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

Timer* Timer_Manager::Find_Timer(const _tchar* pTimerTag)
{
	//auto		iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTag_Finder(pTimerTag));
	auto	iter = m_mapTimer.find(pTimerTag);

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

Timer_Manager* Timer_Manager::Create()
{
	return new Timer_Manager;
}

void Timer_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_mapTimer)
		Safe_Release(Pair.second);	

	m_mapTimer.clear();
}
