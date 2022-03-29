#include "StdAfx.h"
#include "SingleTexture.h"
#include "device.h"

CSingleTexture::CSingleTexture(void)
:m_pTextInfo(NULL)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}
HRESULT CSingleTexture::InsertTexture(const TCHAR* pPath
									  , const TCHAR* pStateKey
									  , const int& iCount)
{
	m_pTextInfo = new TEX_INFO;
	ZeroMemory(m_pTextInfo, sizeof(TEX_INFO));


	if(FAILED( D3DXGetImageInfoFromFile(pPath, &m_pTextInfo->ImageInfo) ) )
	{
		MessageBox(g_hwnd, pPath, L"싱글이미지 정보 얻기 실패!", MB_OK);
		return E_FAIL;
	}

	if(FAILED( D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), pPath,		
										   m_pTextInfo->ImageInfo.Width, m_pTextInfo->ImageInfo.Height,
										   m_pTextInfo->ImageInfo.MipLevels, 0,	
										   m_pTextInfo->ImageInfo.Format, D3DPOOL_MANAGED ,D3DX_DEFAULT,		
										   D3DX_DEFAULT, D3DCOLOR_ARGB(255, 212, 192, 212), &m_pTextInfo->ImageInfo,	NULL, &m_pTextInfo->pTexture) ) )
	{
		MessageBox(g_hwnd, pPath, L"싱글이미지 로드 실패!", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

const TEX_INFO* CSingleTexture::GetTexture(const TCHAR* pKey ,
										   const int& iCount)
{
	return m_pTextInfo;
}

void CSingleTexture::Release(void)
{
	if(m_pTextInfo != NULL)
	{
		m_pTextInfo->pTexture->Release();
		safeDelete(m_pTextInfo);
	}
}