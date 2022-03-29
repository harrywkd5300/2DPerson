#include "StdAfx.h"
#include "DataSubject.h"


IMPLEMENT_SINGLETON(CDataSubject);

CDataSubject::CDataSubject(void)
{
}

CDataSubject::~CDataSubject(void)
{
	Release();
}

list<void*>* CDataSubject::GetDataList(int iMessage)
{
	map<int, list<void*>>::iterator iter;

	iter = m_mapData.find(iMessage);

	if(iter == m_mapData.end())
		return NULL;

	return &iter->second;
}

void CDataSubject::AddData(int iMessage, void* pData)
{
	if (pData != NULL)
	{
		map<int, list<void*>>::iterator iter;

		iter = m_mapData.find(iMessage);

		if(iter == m_mapData.end())
			m_mapData.insert(make_pair(iMessage, list<void*>() ) );

		m_mapData[iMessage].push_back(pData);

		Notify(iMessage, pData);
	}
}

void CDataSubject::RemoveData(int iMessage, void* pData)
{
	map<int, list<void*>>::iterator iter;

	iter = m_mapData.find(iMessage);

	if(iter != m_mapData.end())
	{
		list<void*>& rList = m_mapData[iMessage];

		for(list<void*>::iterator ListIter = rList.begin();
			ListIter != rList.end(); )
		{
			if( (*ListIter) == pData )
			{
				ListIter = rList.erase(ListIter);
			}
			else
				++ListIter;
		}
	}
}

void CDataSubject::Release(void)
{
	map<int, list<void*>>::iterator iter	= m_mapData.begin();
	map<int, list<void*>>::iterator iter_end= m_mapData.end(); 

	for(iter; iter != iter_end; ++iter)
	{
		iter->second.clear();
	}
	m_mapData.clear();
}

