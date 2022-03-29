#pragma once
#include "scene.h"

class CStage :
	public CScene
{
	vector<UNIT_DATA*>	m_Monster;
	vector<INFO*>		m_vecObject;

public:
	CStage(void);
	virtual ~CStage(void);

public:
	void LoadMonster();
	void LoadObject();

public:
	vector<INFO*>*	GetGameObject(void)
	{
		return &m_vecObject;
	}

public:
	virtual HRESULT	Initialize();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();
};
