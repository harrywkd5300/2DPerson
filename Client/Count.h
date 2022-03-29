#pragma once
#include "GameObject.h"
class CCount :
	public CGameObject
{
	float m_fAddCount;
public:
	CCount();
	virtual ~CCount();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);
};

