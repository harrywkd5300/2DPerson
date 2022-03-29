#pragma once

#include "Include.h"

class CObserver;
class CSubject
{
protected:
	list<CObserver*>	m_listObserver;

public:
	virtual void AddObserver(CObserver* pObserver);

	virtual void RemoveObserver(CObserver* pObserver);

	virtual void Notify(int iMessage, void* pData = NULL);

	virtual void Release();

public:
	CSubject(void);
	virtual ~CSubject(void);
};
