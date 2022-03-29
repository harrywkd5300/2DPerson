#pragma once

#include "Include.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);
private:
	map<const TCHAR*, CTexture*>		m_mapTexture;

public:
	wstring			m_wstrCurrentPath;
	int				m_pileCount;
	int				m_pileCT;

private:
	CTextureMgr(void);
public:
	~CTextureMgr(void);

public:
	HRESULT	InsertTexture(const TCHAR* pFilePath,		
		const TCHAR* pObjKey,		
		eTextureType eType,		
		const TCHAR* pStateKey = L"",		
		const int& iCount = 0);			

	const TEX_INFO* GetTexture(const TCHAR* pObjKey
							  ,const TCHAR* pStateKey = L""
		                      ,const int& iCount = 0);

	void Release(void);

public:
	HRESULT LoadImagePath(wstring wstrImagePath);
};
