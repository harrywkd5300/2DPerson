#pragma once

#include "Include.h"

class CFrame
{
private:
	explicit CFrame();
	virtual ~CFrame();

public:
	HRESULT	Initiarize(const float& fCallCnt);
	bool Permit_Call(const float& fTimeDelta);

private:
	float		m_fTimeAcc;
	float		m_fTimePerSec;

public: // 생성과 관련된 함수.
	static CFrame* Create(const float& fCallCnt);

public:
	void Release(void);
};

