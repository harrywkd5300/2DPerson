#pragma once

class CStringCmp
{
public:
	CStringCmp(const TCHAR* pKey)
		: m_pKey(pKey) {}

public:
	template <typename T>
	bool operator()(T& dst) // find_if의 단항 조건자.
	{
		return !lstrcmp(dst.first, m_pKey);
	}

private:
	const TCHAR*	m_pKey;
};

template<typename T>
static void safeDelete(T& rData)
{
	if(rData)
	{
		delete rData;
		rData = NULL;
	}
}

template<typename T>
static void safe_release(T& rData)		//댕글리 포인터를 방지하기 위해서 
{
	if (NULL != rData)
	{
		rData->Release();
		rData = NULL;
	}
}

template <typename T>
bool YSort(T& pDst, T& pSrc)
{
	if (pDst->vPos.y + pDst->vSize.y < pSrc->vPos.y + pSrc->vSize.y)
		return true;
	return false;
}

template <typename T>
bool YMonsterSort(T& pDst, T& pSrc)
{
	//if (pDst->GetUnitInfo()->vPos.y + pDst->GetUnitInfo()->vSize.y < pSrc->GetUnitInfo()->vPos.y + pSrc->GetUnitInfo()->vSize.y)
	//	return true;
	return pDst->GetUnitInfo()->vPos.y + pDst->GetUnitInfo()->vSize.y < pSrc->GetUnitInfo()->vPos.y + pSrc->GetUnitInfo()->vSize.y;
}