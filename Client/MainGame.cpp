#include "StdAfx.h"
#include "MainGame.h"
#include "Device.h"
#include "Scenemgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include <time.h>
#include "SoundMgr.h"

CMainGame::CMainGame(void)
:m_pDevice(CDevice::GetInstance())
,m_fTime(0.f), m_iCount(0)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * MIN_STR);
	D3DXMatrixIdentity(&m_matTrans);
}

CMainGame::~CMainGame(void)
{
	Release();
}
HRESULT CMainGame::initirize()
{
	CSoundMgr::GetInstance()->LoadSoundFile();
	CTimeMgr::GetInstance()->InitTime();

	if (FAILED(m_pDevice->Init3D()))
	{
		ERROR_MSG(L"장치 초기화 실패!!");
		return E_FAIL;
	}	

	CTextureMgr::GetInstance()->Release();

	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Image/Texture/UI/GersangLogo/%d.png"
		, L"UI"
		, TEX_MULTI
		, L"GersangLogo"
		, 1)))
	{
		ERROR_MSG(L"싱글 이미지 로드 실패!");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Image/Texture/UI/Login/UI%d.png"
		, L"UI"
		, TEX_MULTI
		, L"Login"
		, 10)))
	{
		ERROR_MSG(L"싱글 이미지 로드 실패!");
		return E_FAIL;
	}

	CObjMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_LOGO);
	return S_OK;
}
int	CMainGame::Update()
{
	srand(unsigned(time(NULL)));
	CSoundMgr::GetInstance()->UpdateSound();
	CTimeMgr::GetInstance()->SetTime();
	CKeyMgr::GetInstance()->KeyCheck();
	CSceneMgr::GetInstance()->Update();
	return 0;
}
void CMainGame::Render()
{
	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();

	FPS_Render();

	m_pDevice->Render_End();
}
void CMainGame::Release()
{
	m_pDevice->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	//CObjMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}

void CMainGame::FPS_Render()
{
	D3DXMatrixTranslation(&m_matTrans, 30, 30, 0.f);
	++m_iCount;

	m_fTime += GET_TIME;

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;

		wsprintf(m_szFps, L"FPS : %d", m_iCount);

		m_iCount = 0;
	}

	m_pDevice->GetSprite()->SetTransform(&m_matTrans);

	m_pDevice->GetFont()->DrawTextW(
		m_pDevice->GetSprite(),
		m_szFps,
		lstrlen(m_szFps),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 0, 0, 0)
		);
}