#pragma once
#include "GameObject.h"
class CShop :
	public CGameObject
{
	vector<OBJECT*> m_vecItem;
	bool    m_isDead;
	bool	m_bSelect;


	TCHAR   wstateKey[MIDDLE_STR];

public:
	CShop();
	virtual ~CShop();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void LoadObject();
	void Keyinput();
	void BuyItem(OBJECT* pObject);
	int ShopIndex(const D3DXVECTOR3& vPos);
	bool Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject);
};

