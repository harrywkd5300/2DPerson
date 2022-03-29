#pragma once

#include "Include.h"

class CScene abstract
{
public:
	CScene(void);
	virtual ~CScene(void);

public:
	virtual HRESULT	Initialize(void)PURE;
	virtual int		Update(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;
};
