#pragma once
#include "GameObject.h"
class CThunder :
	public CGameObject
{
public:
	CThunder();
	virtual ~CThunder();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

	void MyFrameCheck();
};

