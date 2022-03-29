#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice);
private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3dDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

public:
	CDevice(void);
	~CDevice(void);

public:
	LPDIRECT3DDEVICE9 GetDevice()
	{
		return m_pD3dDevice;
	}
	LPD3DXSPRITE GetSprite()
	{
		return m_pSprite;
	}
	LPD3DXFONT GetFont()
	{
		return m_pFont;
	}

public:
	HRESULT Init3D();
	void	SerParameters(D3DPRESENT_PARAMETERS& d3dp);
	void	Render_Begin(void);
	void	Render_End(void);
	void	Render_End(HWND& _hWnd);
	void	Release(void);
};
