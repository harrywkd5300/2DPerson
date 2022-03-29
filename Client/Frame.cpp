#include "stdafx.h"
#include "Frame.h"


CFrame::CFrame()
	: m_fTimeAcc(0.f)
{
}


CFrame::~CFrame()
{
}

HRESULT CFrame::Initiarize(const float& fCallCnt)
{
	m_fTimePerSec = 1.f / fCallCnt;

	return NOERROR;
}


bool CFrame::Permit_Call(const float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	// 일초에 지정한 갯수만큼만 트루를 리턴한다.
	if (m_fTimePerSec <= m_fTimeAcc)
	{
		m_fTimeAcc = 0.f;
		return true;
	}

	return false;
}


CFrame* CFrame::Create(const float& fCallCnt)
{
	CFrame*		pInstance = new CFrame();

	if (FAILED(pInstance->Initiarize(fCallCnt)))
	{
		MessageBox(0, L"Create Created Failed", L"System Error", MB_OK);
		safe_release(pInstance);
	}
	return pInstance;

}

void CFrame::Release(void)
{
	delete this;
}
