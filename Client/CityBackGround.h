#pragma once
#include "GameObject.h"

class CCityBackGround :
	public CGameObject
{
	vector<OBJECT*>	m_vecCity;
	int		m_iSelectIndex;
	bool	m_bOpenShop;
	bool	m_bOpenItemShop;

public:
	const vector<OBJECT*>*	GetCity(void)
	{
		return &m_vecCity;
	}

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void LoadCity();
	void KeyInput();
	void SelectObject();
	bool Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject);
	int GetCityIndex(const D3DXVECTOR3& vPos);
	bool GetOpenItemShop() { return  m_bOpenItemShop ; }

public:
	void SetView(bool view) { m_bOpenShop = view;	}
	void SetItemView(bool view) { m_bOpenItemShop = view; }


public:
	CCityBackGround();
	virtual ~CCityBackGround();
};

