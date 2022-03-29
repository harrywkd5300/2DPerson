#pragma once

#include "Include.h"

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr);
private:
	CScene*		m_pScene;
private:
	eSceneType	m_eType;
	bool		m_bChangeScene;

public:
	void SetScene(eSceneType eType);
	void SetChangeScene(eSceneType eSceneType);

public:
	eSceneType& GetScene() { return m_eType;  }

public:
	HRESULT	Initialize(void);
	int		Update(void);
	void	Render(void);
	void	Release(void);

private:
	CSceneMgr(void);
public:
	~CSceneMgr(void);
};
