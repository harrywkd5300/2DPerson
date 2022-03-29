#pragma once

#include "Subject.h"

class CDataSubject :
	public CSubject
{
	DECLARE_SINGLETON(CDataSubject);

private:
	map<int, list<void*>>	m_mapData;

public:
	list<void*>*	GetDataList(int iMessage);

public:
	void AddData(int iMessage, void* pData);
	void RemoveData(int iMessage, void* pData);
	void Release(void);


public:
	CDataSubject(void);
	virtual ~CDataSubject(void);
};
