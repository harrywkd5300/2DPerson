#pragma once

#include "Include.h"

class CMaingame
{
public:
	HRESULT initirize();
	int		Update();
	void	Render();
	void	Release();

public:
	void FPS_Render();

public:
	CMaingame();
	~CMaingame();
};

