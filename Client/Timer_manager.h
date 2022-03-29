#pragma once

#include "Include.h"

class CTimer;
class CTimer_manager
{
	DECLARE_SINGLETON(CTimer_manager)

private:
	explicit CTimer_manager();
	virtual ~CTimer_manager();

public:
	float Get_TimeDelta(const TCHAR* pTimerTag);
public:
	HRESULT Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer);
	void Compute_Timer(const TCHAR* pTimerTag);

private:
	map<const TCHAR*, CTimer*>		m_mapTimer;
	typedef map<const TCHAR*, CTimer*>	MAPTIMER;

private:
	CTimer*	Find_Timer(const TCHAR* pTimerTag);

public:
	void Release();


};

