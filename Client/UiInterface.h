#pragma once
#include "gameobject.h"

class CObserver;
class CUiInterface :
	public CGameObject
{
protected:
	CObserver* m_pObserver;

public:
	CUiInterface(void);
	virtual ~CUiInterface(void);
};
