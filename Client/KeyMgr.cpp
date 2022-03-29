#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr);

CKeyMgr::CKeyMgr(void)
{
}

CKeyMgr::~CKeyMgr(void)
{
}
void CKeyMgr::KeyCheck()
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LEFT))
		m_dwKey |= KEY_LEFT;
	if(GetAsyncKeyState(VK_RIGHT))
		m_dwKey |= KEY_RIGHT;
	if(GetAsyncKeyState(VK_UP))
		m_dwKey |= KEY_UP;
	if(GetAsyncKeyState(VK_DOWN))
		m_dwKey |= KEY_DOWN;
	if(GetAsyncKeyState(VK_LBUTTON))
		m_dwKey |= LBUTTON;
	if(GetAsyncKeyState(VK_RBUTTON))
		m_dwKey |= RBUTTON;
	if (GetAsyncKeyState(VK_RETURN))
		m_dwKey |= ENTER;
	if (GetAsyncKeyState('P'))
		m_dwKey |= P;
	if (GetAsyncKeyState('I'))
		m_dwKey |= I;
	if (GetAsyncKeyState('A'))
		m_dwKey |= A;
	if (GetAsyncKeyState('S'))
		m_dwKey |= S;
}