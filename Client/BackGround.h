#pragma once
#include "GameObject.h"

class CBackGround :
	public CGameObject
{
	vector<TILE*>	m_vecTile;

private:
	POINT	m_ptTileRender;
	bool    m_TILEMAP;

public:
	const vector<TILE*>*	GetTile(void)
	{
		return &m_vecTile;
	}

public:
	CBackGround(void);
	virtual ~CBackGround(void);

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void LoadTile();
	void MouseScrollCheck();
	void SetTilemap(bool view) { m_TILEMAP = view; }
	
public:
	bool GetTilemap() { return m_TILEMAP; }
};
