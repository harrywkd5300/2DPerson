#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
private:
	TEX_INFO*		m_pTextInfo;

public:
	virtual HRESULT InsertTexture(const TCHAR* pPath,				
								  const TCHAR* pStateKey = L"",
							      const int& iCount = 0);

	virtual const TEX_INFO* GetTexture(const TCHAR* pKey = L"", 
									   const int& iCount = 0);

	virtual void Release(void);

public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};
