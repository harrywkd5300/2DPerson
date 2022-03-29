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

public: // ������ ���õ� �Լ�.
	static CFrame* Create(const float& fCallCnt);

public:
	void Release(void);
};

