#pragma once

#include "Include.h"


class CGameObject;
class CBridge
{
protected:
	CGameObject*	m_pGameObject;

	wstring			m_wstrStateKey;
	FRAME			m_tFrame;

public:
	void SetGameObject(CGameObject* pObject)
	{
		m_pGameObject = pObject;
	}

public:
	virtual HRESULT	Initialize(void)PURE;
	virtual int		Update(INFO& rInfo)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;

public:
	CBridge(void);
	virtual ~CBridge(void);
};
