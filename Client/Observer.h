#pragma once

#include "Include.h"

class CObserver abstract
{

public:
	virtual void Update(int iMessage, void* pData)PURE;

public:
	CObserver(void);
	virtual ~CObserver(void);
};
