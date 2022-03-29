#include "stdafx.h"
#include "Timer_manager.h"

#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_manager)

CTimer_manager::CTimer_manager()
{
}


CTimer_manager::~CTimer_manager()
{
}

float CTimer_manager::Get_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer*			pTimer = NULL;

	pTimer = Find_Timer(pTimerTag);

	if (NULL == pTimer)
		return 0.0f;

	return pTimer->Get_TimeDelta();
}

HRESULT CTimer_manager::Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer)
{
	if (NULL != Find_Timer(pTimerTag))
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return NOERROR;
}

void CTimer_manager::Compute_Timer(const TCHAR* pTimerTag)
{
	CTimer*			pTimer = NULL;

	pTimer = Find_Timer(pTimerTag);

	if (NULL == pTimer)
		return;

	pTimer->Computer_Timer();

}

CTimer* CTimer_manager::Find_Timer(const TCHAR* pTimerTag)
{
	MAPTIMER::iterator iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), STRING_COMPARE(pTimerTag));

	if (iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}

void CTimer_manager::Release(void)
{
	delete this;
}
