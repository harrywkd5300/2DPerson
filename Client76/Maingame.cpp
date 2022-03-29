#include "stdafx.h"
#include "Maingame.h"


CMaingame::CMaingame()
{
}


CMaingame::~CMaingame()
{
	Release();
}
HRESULT CMaingame::initirize()
{
	/*CTimeMgr::GetInstance()->InitTime();

	if (FAILED(m_pDevice->Init3D()))
	{
		ERROR_MSG(L"장치 초기화 실패!!");
		return E_FAIL;
	}
	CObjMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_LOGO);*/
	return S_OK;
}
int	CMaingame::Update()
{
	//CTimeMgr::GetInstance()->SetTime();
	//CKeyMgr::GetInstance()->KeyCheck();
	//CSceneMgr::GetInstance()->Update();
	return 0;
}
void CMaingame::Render()
{
	//m_pDevice->Render_Begin();

	//CSceneMgr::GetInstance()->Render();

	//FPS_Render();

	//m_pDevice->Render_End();
}
void CMaingame::Release()
{
	//m_pDevice->DestroyInstance();
	//CSceneMgr::GetInstance()->DestroyInstance();
	//CTextureMgr::GetInstance()->DestroyInstance();
	//CTimeMgr::GetInstance()->DestroyInstance();
	//CKeyMgr::GetInstance()->DestroyInstance();
}

void CMaingame::FPS_Render()
{
	/*D3DXMatrixTranslation(&m_matTrans, 30, 30, 0.f);
	++m_iCount;

	m_fTime += GET_TIME;
*/
	//if (m_fTime > 1.f)
	//{
	//	m_fTime = 0.f;

	//	wsprintf(m_szFps, L"FPS : %d", m_iCount);

	//	m_iCount = 0;
	//}

	//m_pDevice->GetSprite()->SetTransform(&m_matTrans);

	//m_pDevice->GetFont()->DrawTextW(
	//	m_pDevice->GetSprite(),
	//	m_szFps,
	//	lstrlen(m_szFps),
	//	NULL,
	//	NULL,
	//	D3DCOLOR_ARGB(255, 0, 0, 0)
	//);
}
