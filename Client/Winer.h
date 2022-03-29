#pragma once
#include "GameObject.h"

class CWiner :
	public CGameObject
{
public:
	CWiner();
	virtual ~CWiner();

public:
	virtual HRESULT	Initialize();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

	void FrameCheck();
};

