#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr);

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}
HRESULT CTextureMgr::InsertTexture(const TCHAR* pFilePath,
								   const TCHAR* pObjKey, 
								   eTextureType eType,  
								   const TCHAR* pStateKey,
								   const int& iCount)
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObjKey));

	if(iter == m_mapTexture.end())
	{
		CTexture* pTexture = NULL;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED( pTexture->InsertTexture(pFilePath, pStateKey, iCount) ))
		{
			MessageBox(g_hwnd, pObjKey, L"Error!!", MB_OK);
			return E_FAIL;
		}

		TCHAR* pKey = new TCHAR[MAX_PATH];
		lstrcpy(pKey, pObjKey);

		m_mapTexture.insert( make_pair(pKey, pTexture) );
	}
	else
	{
		if (eType == TEX_MULTI)
			iter->second->InsertTexture(pFilePath, pStateKey, iCount);
	}


	return S_OK;
}

const TEX_INFO* CTextureMgr::GetTexture(const TCHAR* pObjKey 
									   ,const TCHAR* pStateKey /*= L"" */
									   ,const int& iCount /*= 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter;

	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), STRING_COMPARE(pObjKey));

	if(iter == m_mapTexture.end())
		return NULL;

	return iter->second->GetTexture(pStateKey, iCount);
}

void CTextureMgr::Release(void)
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), DeleteMap() );
	m_mapTexture.clear();
}

HRESULT CTextureMgr::LoadImagePath(wstring wstrImagePath)
{
	wifstream openFile;
	openFile.open(wstrImagePath.c_str(), ios::in);

	TCHAR szObjKey[MIN_STR] = L"";
	TCHAR szStateKey[MIN_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_PATH] = L"";

	Release();

	if (openFile.is_open())
	{
		int iCount = 0;
		while (!openFile.eof())
		{
			openFile.getline(szObjKey, MIN_STR, '|');
			openFile.getline(szStateKey, MIN_STR, '|');
			openFile.getline(szCount, MIN_STR, '|');
			openFile.getline(szPath, MAX_PATH);

			int iCount = _ttoi(szCount);

			if (FAILED(InsertTexture(szPath, szObjKey, TEX_MULTI, szStateKey, iCount)))
			{
				ERROR_MSG(L"로드실패!!!");
				return E_FAIL;
			}
		}
	}
	openFile.close();

	return S_OK;
}