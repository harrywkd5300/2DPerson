#include "stdafx.h"
#include "City.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Player.h"
#include "CityBackGround.h"
#include "SoundMgr.h"
#include "Mouse.h"

CCity::CCity()
{

}
CCity::~CCity()
{
	Release();
}

HRESULT	CCity::Initialize()
{
	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CCityBackGround>::CreateGameObject(),
		OBJ_TYPE_BACKGROUND);

	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMouse>::CreateGameObject(),
		OBJ_TYPE_MOUSE);

	CSoundMgr::GetInstance()->PlayBGM(L"Town.wav", CSoundMgr::Channel_BGM);

	return S_OK;
}

int CCity::Update()
{
	CObjMgr::GetInstance()->Update();
	return 0;
}

void CCity::Render()
{
	CObjMgr::GetInstance()->Render();
}

void CCity::Release()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		safeDelete(m_vecObject[i]);
	}
	m_vecObject.clear();
	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetPlayerView(true);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_BACKGROUND);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_MOUSE);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);
}

//void CCity::LoadObject()
//{
//
//	DWORD dwByte = 0;
//
//	HANDLE hFile = CreateFile(
//		L"../Data/BattleObject.dat",
//		GENERIC_READ,
//		NULL,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
//		NULL
//	);
//
//
//	while (true)
//	{
//		INFO* pCityData = new INFO;
//
//		ZeroMemory(pCityData, sizeof(INFO));
//
//		ReadFile(hFile, pCityData, sizeof(INFO), &dwByte, NULL);
//
//		if (dwByte == 0)
//		{
//			safeDelete(pCityData);
//			break;
//		}
//		m_vecObject.push_back(pCityData);
//	}
//
//}	CloseHandle(hFile);