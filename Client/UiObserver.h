#pragma once
#include "observer.h"

class CUiObserver :
	public CObserver
{
private:
	UNIT_DATA	m_tData;
	D3DXMATRIX  m_matPlayerMatrix;

public:
	const UNIT_DATA*  GetUnitData()
	{
		return &m_tData;
	}
	const D3DXMATRIX* GetMatrix()
	{
		return &m_matPlayerMatrix;
	}

public:
	virtual void Update(int iMessage, void* pData);

public:
	CUiObserver(void);
	virtual ~CUiObserver(void);
};
