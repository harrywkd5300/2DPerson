#include "stdafx.h"
#include "Battle.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "BackGround.h"
#include "Tower.h"
#include "Monster.h"
#include "Player.h"
#include "Portrait.h"
#include "Mouse.h"
#include "Dragon.h"
#include "FireBird.h"
#include "Turtle.h"
#include "Tiger.h"
#include "LeeSoonsin.h"
#include "Sunbiyean.h"
#include "Jiguk.h"
#include "Guangmok.h"
#include "Jeungjang.h"
#include "Damoon.h"
#include "MainUI.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
#include "Winer.h"

CBattle::CBattle()
	:iCount(0), m_fAddTime(0.f), m_bEnd(true)
{
}


CBattle::~CBattle()
{
	Release();
}

HRESULT	CBattle::Initialize()
{
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
		CFactory<CMouse>::CreateGameObject(),
		OBJ_TYPE_MOUSE);

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 MonsterPos;
		int type;
		if (i == 0)
		{
			MonsterPos = D3DXVECTOR3(1600.f, 350.f, 0.f); //16
			type = 0;
		}
		else if (i == 1)
		{
			MonsterPos = D3DXVECTOR3(1500.f, 550.f, 0.f); //17
			type = 1;
		}
		else if (i == 2)
		{
			MonsterPos = D3DXVECTOR3(1700.f, 550.f, 0.f); //16
			type = 2;
		}
		else if (i == 3)
		{
			MonsterPos = D3DXVECTOR3(1500.f, 350.f, 0.f); //17
			type = 3;
		}

		CObjMgr::GetInstance()->AddGameObject(
			CFactory<CMonster>::CreateGameObject(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetUnitInfo(), MonsterPos, type),
			OBJ_TYPE_MONSTER);
	}


	//CObjMgr::GetInstance()->AddGameObject(
	//	CFactory<CMouse>::CreateGameObject(),
	//	OBJ_TYPE_MOUSE);

	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetUnitInfo()->vPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos;
	lstrcpy(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetUnitInfo()->wstrObjKey, L"Player");


	CObjMgr::GetInstance()->AddGameObject(
		CFactory<CMainUI>::CreateGameObject(),
		OBJ_TYPE_UI);

	CSoundMgr::GetInstance()->PlayBGM(L"Battle.mp3", CSoundMgr::Channel_BGM);

	return S_OK;
}

int CBattle::Update()
{
	if (m_bEnd)
	{
		if (CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).empty())
		{
			m_bEnd = false;

			CObjMgr::GetInstance()->AddGameObject(
				CFactory<CWiner>::CreateGameObject(),
				OBJ_TYPE_UI);
			return 0;
		
		}
		list<CGameObject*>::iterator iter = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).begin();
		list<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).end();

		if (!CObjMgr::GetInstance()->GetList(OBJ_TYPE_MOUSE).empty())
		{
			if (CObjMgr::GetInstance()->FindGameObject<CMouse>()->m_bClick)
			{
				for (; iter != iter_end; ++iter)
				{
					RECT rc = {};
					if (IntersectRect(&rc, &(CObjMgr::GetInstance()->FindGameObject<CMouse>()->GetRect()), &((*iter)->GetCollRect())))
						(*iter)->SetClickUnit(true);
					else
						(*iter)->SetClickUnit(false);
				}
			}
		}

		CollisionCheck(&(CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER)), &(CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER)));
		CollisionCheck(&(CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER)), &(CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT)));
		CollisionCheck(&(CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT)), &(CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT)));
		//CollisionCheck(&(CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER)), &(CObjMgr::GetInstance()->GetList(OBJ_TYPE_PLAYER)));
		//CollisionCheck(&(CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT)), &(CObjMgr::GetInstance()->GetList(OBJ_TYPE_PLAYER)));
		iter = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).begin();
		iter_end = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).end();

		m_fAddTime += GET_TIME * 3.f;
		if (m_fAddTime > 5.f)
		{
			m_fAddTime = 0.f;
			for (; iter != iter_end; ++iter)
			{
				list<CGameObject*>::iterator iter_mon = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).begin();
				list<CGameObject*>::iterator iter_mon_end = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).end();
				for (; iter_mon != iter_mon_end; ++iter_mon)
				{
					if (CheckCollSphere((*iter), (*iter_mon)))
					{
						if ((*iter)->GetAttack())
						{
							(*iter_mon)->SetiHp(10);
							(*iter_mon)->SetHitTime(false);
							(*iter_mon)->SetAttack(false);
						}
						if ((*iter_mon)->GetAttack())
						{
							(*iter)->SetiHp(5);
							(*iter)->SetHitTime(false);
							(*iter)->SetAttack(false);
						}
					}
				}
			}
		}
	}
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CBattle::Render()
{
	CObjMgr::GetInstance()->Render();
}

void CBattle::Release()
{
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		safeDelete(m_vecObject[i]);
	}
	m_vecObject.clear();

	CObjMgr::GetInstance()->FindGameObject<CPortrait>()->SetPortrait(false);
	//CObjMgr::GetInstance()->FindGameObject<CMonster>()->SetMoveMonster(true);
	//CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_MOUSE);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_UNIT);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_UI);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_OBJECT);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_BACKGROUND);
	CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_MONSTER);
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);
}

D3DXVECTOR3 CBattle::GetMonsterPos(int iA)
{
	D3DXVECTOR3 vPos;
	if (iA == 0)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 500.f, 200.f, 0.f);
	else if (iA == 1)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 400.f, 150.f, 0.f);
	else if (iA == 2)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 300.f, 300.f, 0.f);
	else if (iA == 3)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 600.f, 400.f, 0.f);
	else if (iA == 4)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 200.f, 400.f, 0.f);
	else if (iA == 5)
		vPos = D3DXVECTOR3((TILEX * TILECX) - 500.f, 300.f, 0.f);

	return vPos;
}

bool CBattle::CheckCollSphere(CGameObject* pDst, CGameObject* pSrc)
{
	// 두 객체의 반지름 합 구하기.
	int iSumRad = int((pDst->GetUnitInfo()->vSize.x - 30.f) + (pSrc->GetUnitInfo()->vSize.x - 30.f));

	// 두 객체의 중점 간 거리 구하기.
	int iWidth  = int(pDst->GetUnitInfo()->vPos.x - pSrc->GetUnitInfo()->vPos.x);
	int iHeight = int(pDst->GetUnitInfo()->vPos.y - pSrc->GetUnitInfo()->vPos.y);

	// 피라고라스를 이용한 두 점 사이의 거리 구하기.
	int iDist = (int)sqrtf(float((iWidth) * (iWidth) + (iHeight) * (iHeight)));

	// 두 객체의 반지름 합이 중점보다 커진다면 충돌 되었다!
	if (iSumRad >= iDist)
		return true;

	return false;
}

bool CBattle::CheckCollRect(CGameObject* pDst, CGameObject* pSrc, int* pMoveX, int* pMoveY)
{
	// 두 사각형의 가로 반지름 합과 세로 반지름 합을 구한다.
	int iWidth = 40;
	int iHeight = 40;
	// 두 사각형의 가로 중점거리와 세로 중점거리를 구한다.
	int iDistX = int(abs(pDst->GetUnitInfo()->vPos.x - pSrc->GetUnitInfo()->vPos.x));
	int iDistY = int(abs(pDst->GetUnitInfo()->vPos.y - pSrc->GetUnitInfo()->vPos.y));

	if ((iWidth > iDistX) && (iHeight > iDistY))
	{
		// 가로축과 세로축으로 밀려날 거리를 구한다.
		*pMoveX = iWidth  - iDistX;
		*pMoveY = iHeight - iDistY;

		return true;
	}

	return false;
}


void CBattle::LoadObject()
{
	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(
		L"../Data/BattleObject.dat",
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

void CBattle::CollisionCheck(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	// x, y축으로 밀려날 거리.
	int iMoveX = 0;
	int iMoveY = 0;

	for (; dst_begin != dst_end; ++dst_begin)
	{
		// 이미 죽은 객체라면 건너 뛴다.
		if (true == (*dst_begin)->GetDeadState())
			continue;

		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{
			// 이미 죽은 객체라면 건너 뛴다.
			if (true == (*src_begin)->GetDeadState())
				continue;
			if ((*dst_begin)->GetUnitInfo()->vPos == (*src_begin)->GetUnitInfo()->vPos)
				continue;

			// 사각형 충돌
			if (CheckCollRect((*dst_begin), (*src_begin), &iMoveX, &iMoveY))
			{
				// 밀려나갈 축이 짧은 쪽으로 밀려나간다!
				if (iMoveX > iMoveY)
				{
					int iX = int((*dst_begin)->GetUnitInfo()->vPos.x);
					int iY = int((*dst_begin)->GetUnitInfo()->vPos.y);

					if ((*src_begin)->GetUnitInfo()->vPos.y > iY)
						iMoveY *= -1;

					(*dst_begin)->SetUnitPos(float(iX), float(iY + iMoveY));
				}
				else
				{
					int iX = int((*dst_begin)->GetUnitInfo()->vPos.x);
					int iY = int((*dst_begin)->GetUnitInfo()->vPos.y);

					if ((*src_begin)->GetUnitInfo()->vPos.x > iX)
						iMoveX *= -1;

					(*dst_begin)->SetUnitPos(float(iX + iMoveX), float(iY));
				}
			}
		}
	}
}