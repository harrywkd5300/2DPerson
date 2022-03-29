#pragma once
#include "GameObject.h"
class CTower :
	public CGameObject
{

private:
	int m_iSelectIndex;

public:
	bool bObjectClick;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	bool Picking(const D3DXVECTOR3& vPos);
	void SelectObject();

public:
	CTower();
	virtual ~CTower();
};

