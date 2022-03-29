#pragma once
#include "GameObject.h"
class CBattleCount :
	public CGameObject
{
	bool m_bSelectPortait;
	int  m_fAddTime;
	float m_fPlusTime;

public:
	CBattleCount();
	virtual ~CBattleCount();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void MyFrameCheck();

};

