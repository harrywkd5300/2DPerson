#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{

}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}
HRESULT CMultiTexture::InsertTexture(const TCHAR* pPath,					
									 const TCHAR* pStateKey, 
									 const int& iCount)
{
	TCHAR szPath[MAX_STR] = L"";

	vector<TEX_INFO*>	vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, pPath, i);
		TEX_INFO* pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		if( FAILED( D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImageInfo) ) )
			return E_FAIL;

		if( FAILED( D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szPath,
			pTexInfo->ImageInfo.Width,
			pTexInfo->ImageInfo.Height,
			pTexInfo->ImageInfo.MipLevels,
			0,
			pTexInfo->ImageInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DCOLOR_ARGB(255, 212, 192, 212),
			&pTexInfo->ImageInfo,
			NULL,
			&pTexInfo->pTexture
			) ) )
		{
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
	}

	TCHAR* pKey = new TCHAR[MAX_PATH];
	lstrcpy(pKey, pStateKey);

	m_mapMultiTexture.insert( make_pair(pKey, vecTexture) );

	return S_OK;
}

const TEX_INFO* CMultiTexture::GetTexture(const TCHAR* pKey , 
										  const int& iCount)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;

	iter = find_if(m_mapMultiTexture.begin(), m_mapMultiTexture.end(), STRING_COMPARE(pKey) );

	if(iter == m_mapMultiTexture.end())
		return NULL;

	return iter->second[iCount];
}

void CMultiTexture::Release(void)
{
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter;
	map<const TCHAR*, vector<TEX_INFO*>>::iterator iter_end;

	iter	 = m_mapMultiTexture.begin();
	iter_end = m_mapMultiTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete [] iter->first;

		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			safeDelete(iter->second[i]);
		}
		iter->second.clear();
	}

	m_mapMultiTexture.clear();
}