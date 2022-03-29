#pragma once

#include "Include.h"

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr);
private:
	DWORD m_dwKey;

public:
	CKeyMgr(void);
private:
	~CKeyMgr(void);

public:
	DWORD Getkey() { return m_dwKey; }
	void KeyCheck();
};
