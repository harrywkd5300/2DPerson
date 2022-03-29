#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "Factory.h"
#include "Tower.h"
#include "Monster.h"
#include "Player.h"
#include "Sunbiyean.h"
#include "Portrait.h"
#include "Mouse.h"
#include "MainUI.h"
#include "SteminaBar.h"
#include "SoundMgr.h"

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}

HRESULT	CStage::Initialize()
{
	LoadMonster();
	LoadObject();

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CBackGround>::CreateGameObject(),
		OBJ_TYPE_BACKGROUND);

	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		CObjMgr::GetInstance()->AddGameObject(
			CFactory<CTower>::CreateGameObject(m_vecObject[i]),
			OBJ_TYPE_OBJECT);
	}

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMonster>::CreateGameObject(m_Monster[0]),
		OBJ_TYPE_MONSTER);

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMonster>::CreateGameObject(m_Monster[1]),
		OBJ_TYPE_MONSTER);

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMouse>::CreateGameObject(),
		OBJ_TYPE_MOUSE);

	if (CObjMgr::GetInstance()->GetList()[OBJ_TYPE_PLAYER].empty())
	{
		CObjMgr::GetInstance()->AddGameObject(
			CFactory<CPlayer>::CreateGameObject(),
			OBJ_TYPE_PLAYER);

		CObjMgr::GetInstance()->AddGameObject(
			CFactory<CPortrait>::CreateGameObject(),
			OBJ_TYPE_PORTRAIT);
	}

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMainUI>::CreateGameObject(),
		OBJ_TYPE_UI);

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CSteminaBar>::CreateGameObject(),
		OBJ_TYPE_UI);

	CSoundMgr::GetInstance()->PlayBGM(L"Stage.mp3", CSoundMgr::Channel_BGM);

	return S_OK;
}
int	CStage::Update()
{
	CObjMgr::GetInstance()->Update();
	return 0;
}
void CStage::Render()
{
	CObjMgr::GetInstance()->Render();
}
void CStage::Release()
{
	for (size_t i = 0; i < m_Monster.size(); ++i)
	{
		safeDelete(m_Monster[i]);
	}
	m_Monster.clear();

	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		safeDelete(m_vecObject[i]);
	}
	m_vecObject.clear();

	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_BACKGROUND);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_MONSTER);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_OBJECT);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_MOUSE);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_UI);
	CSoundMgr::GetInstance()->StopAll();
}

void CStage::LoadMonster()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/StageBoss.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
		);


	for (size_t i = 0; i < m_Monster.size(); ++i)
	{
		safeDelete(m_Monster[i]);
	}
	m_Monster.clear();

	while (true)
	{
		UNIT_DATA* pObjectData = new UNIT_DATA;
		ZeroMemory(pObjectData, sizeof(UNIT_DATA));

		ReadFile(hFile, pObjectData, sizeof(UNIT_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			safeDelete(pObjectData);
			break;
		}
		m_Monster.push_back(pObjectData);
	}

	CloseHandle(hFile);
}
void CStage::LoadObject()
{

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/StageObject.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
	);

	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		safeDelete(m_vecObject[i]);
	}
	m_vecObject.clear();

	while (true)
	{
		INFO* pObjectData = new INFO;
		ZeroMemory(pObjectData, sizeof(INFO));

		ReadFile(hFile, pObjectData, sizeof(INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			safeDelete(pObjectData);
			break;
		}
		m_vecObject.push_back(pObjectData);
	}

	CloseHandle(hFile);
}