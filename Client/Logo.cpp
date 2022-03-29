#include "StdAfx.h"
#include "Logo.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CLogo::CLogo(void)
	:m_iLoginCount(0)
{
	memset(&m_Crt, 0, sizeof(CRITICAL_SECTION));
}

CLogo::~CLogo(void)
{		
	Release();
}

HRESULT	CLogo::Initialize()
{
	InitializeCriticalSection(&m_Crt);

	CSoundMgr::GetInstance()->PlayBGM(L"Login.wav", CSoundMgr::Channel_BGM);

	m_hThread = (HANDLE)_beginthreadex(
				NULL,
				0,
				ImageLoadThreadFunc,
				this,
				NULL,
				NULL);

	return S_OK;
}

int	CLogo::Update()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();
	if( KEYPRESS(dwKey, ENTER) )
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		DeleteCriticalSection(&m_Crt);

		CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);
	}
	return 0;
}

void CLogo::Render()
{
	D3DXMATRIX  matWorld, matScale, matTrans;

	const TEX_INFO* pImageTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
											   L"GersangLogo",
											   0);
	
	if (pImageTex == NULL)
	{
		ERROR_MSG(L"로고 이미지 X");
		return;
	}

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matScale);

	CDevice::GetInstance()->GetSprite()->Draw(
		pImageTex->pTexture,
		NULL,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	/////////////////////////////////////////////////////////
	const TEX_INFO* pCountTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
			L"Login",
			CTextureMgr::GetInstance()->m_pileCount);

	if (pCountTex == NULL)
	{
		ERROR_MSG(L"카운트 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, 400.f
		, 100.f
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	float fX = CENTER_POS(pCountTex->ImageInfo.Width);
	float fY = CENTER_POS(pCountTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pCountTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	/////////////////////////////////////////////////////

	D3DXMatrixTranslation(&matTrans, 30, 550, 0);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str(),
		lstrlen(CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str()),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}

void CLogo::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);
}

unsigned int __stdcall CLogo::ImageLoadThreadFunc(void* pArg)
{
	CLogo* pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/SaveData2.txt") ))
	{
		ERROR_MSG(L"멀티텍스쳐 이미지 로드 실패!");
		return E_FAIL;
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"이미지 로드 완료!";

	LeaveCriticalSection(&pLogo->GetCrt());

	return 0;
}