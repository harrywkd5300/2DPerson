#pragma once
#include "GameObject.h"

class CEmployshop :
	public CGameObject
{
	vector<OBJECT*> m_vecEmploy;
	int		m_iSelectIndex;
	bool	m_bSelectEmploy;
	bool	m_isDead;
	bool	m_bSelectPortait;

	float  m_fPlusTime;
	float  m_fAddTime;
	

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CEmployshop();
	virtual ~CEmployshop();

public:
	void LoadObject();
	void BuyEmploy();
	void SelectObject();
	bool Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject);
	int GetCityIndex(const D3DXVECTOR3& vPos);

	void BuyChar(TCHAR  wstrStateKey[MIDDLE_STR]);
	void SellChar(int iIndex);

};

