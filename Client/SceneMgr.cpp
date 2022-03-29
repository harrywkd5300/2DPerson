#include "StdAfx.h"
#include "SceneMgr.h"

#include "Logo.h"
#include "Stage.h"
#include "City.h"
#include "Battle.h"

IMPLEMENT_SINGLETON(CSceneMgr);

CSceneMgr::CSceneMgr(void)
{
	m_pScene		= NULL;
	m_bChangeScene	= false;
	m_eType			= SCENE_MAX;
}
CSceneMgr::~CSceneMgr(void)
{
	Release();
}
void CSceneMgr::SetScene(eSceneType eType)
{
	safeDelete(m_pScene);

	switch(eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;
	case SCENE_STAGE:
		m_pScene = new CStage;
		break;
	case SCENE_CITY:
		m_pScene = new CCity;
		break;
	case SCENE_BATTLE:
		m_pScene = new CBattle;
		break;
	}

	m_pScene->Initialize();

}
void CSceneMgr::SetChangeScene(eSceneType eSceneType)
{
	m_bChangeScene = true;
	m_eType		   = eSceneType;
}

HRESULT	CSceneMgr::Initialize(void)
{
	return S_OK;
}
int	CSceneMgr::Update(void)
{
	if(m_bChangeScene)
	{
		m_bChangeScene = false;
		SetScene(m_eType);
	}
	if(m_pScene)
		m_pScene->Update();

	return 0;
}
void CSceneMgr::Render(void)
{
	if (m_pScene)
	{
		m_pScene->Render();
	}

}
void CSceneMgr::Release(void)
{
	safeDelete(m_pScene);	
}