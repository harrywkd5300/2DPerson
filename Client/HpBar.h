#pragma once
#include "uiinterface.h"

class CHpBar :
	public CUiInterface
{
private:
	int m_iHp;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CHpBar(void);
	virtual ~CHpBar(void);
};
