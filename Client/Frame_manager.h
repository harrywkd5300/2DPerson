#pragma once

#include "Include.h"

class CFrame;
class CFrame_manager
{
	DECLARE_SINGLETON(CFrame_manager)
private:
	explicit CFrame_manager(void);
	virtual ~CFrame_manager(void);


public: //추가하겠다.
	HRESULT Add_Frame(const TCHAR* pFrameTag, const float& fCallCnt);
	bool Permit_Call(const TCHAR* pFrameTag, const float& fTimeDelta);
private:
	map<const TCHAR*, CFrame*>			m_mapFrame;
	typedef map<const TCHAR*, CFrame*>	MAPFRAME;
private:
	CFrame* Find_Frame(const TCHAR* pFrameTag);
public:
	void Release(void);
};

