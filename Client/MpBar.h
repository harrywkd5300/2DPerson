#pragma once
#include "uiinterface.h"

class CMpBar :
	public CUiInterface
{
private:
	int m_iMp;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CMpBar(void);
	virtual ~CMpBar(void);
};
