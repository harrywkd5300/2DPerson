#pragma once
#include "GameObject.h"
class CBomb :
	public CGameObject
{
public:
	CBomb();
	virtual ~CBomb();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

	void MyFrameCheck();
};

