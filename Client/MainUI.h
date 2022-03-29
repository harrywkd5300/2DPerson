#pragma once
#include "GameObject.h"
class CMainUI :
	public CGameObject
{

public:
	CMainUI();
	virtual ~CMainUI();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);
};

