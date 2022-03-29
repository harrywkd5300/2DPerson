#pragma once

#include "Include.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER	m_FrameTime;
	LARGE_INTEGER	m_FixTime;
	LARGE_INTEGER	m_LastTime;
	LARGE_INTEGER	m_CpuTick;

	float			m_fTime;
	float			m_fGlobalTime;

public:
	float	GetTime()		{ return m_fTime;		}
	float	GetGlobalTime() { return m_fGlobalTime; }

	void	InitTime();
	void	SetTime();
	void	Release();

private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};
