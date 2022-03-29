#pragma once
#include "Scene.h"

class CCity :
	public CScene
{
	vector<INFO*>		m_vecObject;
public:
	virtual HRESULT	Initialize();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

public:
	vector<INFO*>*	GetGameObject(void)
	{
		return &m_vecObject;
	}

	//void LoadObject();
public:
	CCity();
	virtual ~CCity();
};

