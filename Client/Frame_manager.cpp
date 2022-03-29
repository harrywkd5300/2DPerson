#include "stdafx.h"
#include "Frame_manager.h"
#include "Frame.h"


IMPLEMENT_SINGLETON(CFrame_manager)
CFrame_manager::CFrame_manager()
{
}


CFrame_manager::~CFrame_manager()
{
}

HRESULT CFrame_manager::Add_Frame(const TCHAR* pFrameTag, const float& fCallCnt)
{
	if (NULL != Find_Frame(pFrameTag))
		return E_FAIL;

	CFrame*		pFrame = CFrame::Create(fCallCnt);
	if (NULL == pFrame)
		return E_FAIL;

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));

	return NOERROR;
}

bool CFrame_manager::Permit_Call(const TCHAR* pFrameTag, const float& fTimeDelta)
{
	CFrame*			pFrame = NULL;

	pFrame = Find_Frame(pFrameTag);

	if (NULL == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

CFrame* CFrame_manager::Find_Frame(const TCHAR* pFrameTag)
{
	MAPFRAME::iterator iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), STRING_COMPARE(pFrameTag));

	if (iter == m_mapFrame.end())
		return NULL;

	return iter->second;
}

void CFrame_manager::Release(void)
{
	delete this;
}
