#pragma once

#include "Include.h"

class CDevice;
class CMainGame
{
	CDevice* m_pDevice;
private:
	float		m_fTime;
	int			m_iCount;
	TCHAR		m_szFps[MIN_STR];
	D3DXMATRIX  m_matTrans;

public:
	CMainGame(void);
	~CMainGame(void);

public:
	HRESULT initirize();
	int		Update();
	void	Render();
	void	Release();

public:
	void FPS_Render();
};
