#pragma once
#include "GameObject.h"
class CBattleBackGround :
	public CGameObject
{
	vector<TILE*>	m_vecTile;
	vector<INFO*>	m_vecCity;

	D3DXVECTOR3 m_BattleScroll;

private:
	POINT	m_ptTileRender;

public:
	const vector<TILE*>*	GetTile(void)
	{
		return &m_vecTile;
	}
	
	const vector<INFO*>*	GetCity(void)
	{
		return &m_vecCity;
	}
	D3DXVECTOR3& GetScroll()
	{
		return m_BattleScroll;
	}


public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void LoadTile();
	void LoadObject();
	void MouseScrollCheck();


public:
	CBattleBackGround();
	virtual ~CBattleBackGround();
};

