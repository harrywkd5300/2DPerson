#pragma once
#include "GameObject.h"

class CMouseClick :
	public CGameObject
{
public:
	CMouseClick();
	virtual ~CMouseClick();

	void FrameCheck();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);
};

