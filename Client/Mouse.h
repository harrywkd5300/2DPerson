#pragma once

#include "GameObject.h"

class CMouse :
	public CGameObject
{
	wstring wstrObjKey;
	wstring wstrStateKey;

	D3DXVECTOR3 m_clickPos;
	D3DXVECTOR3 m_clickAgoPos;

	D3DXVECTOR3 m_clickUIAgoPos;


	bool	m_bMakeRect;
	float	m_bClickTime;

public:
	bool    m_bClick;
	bool	m_bClick2;
	bool	m_bFirstClick;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void FrameCheck();
	void SetClick2(bool Click) { m_bClick2 = Click; }

public:
	void SetStatekey(wstring statekey) { wstrStateKey = statekey; }

	D3DXVECTOR3& GetAgoPos() { return m_clickUIAgoPos; }


public:
	static D3DXVECTOR3 GetMousePos()
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hwnd, &ptMouse);

		return D3DXVECTOR3(float(ptMouse.x),
						   float(ptMouse.y),
						   0.f);
	}
};
