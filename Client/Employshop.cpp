#include "stdafx.h"
#include "Employshop.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Portrait.h"
#include "TimeMgr.h"
#include "CityBackGround.h"
#include "Player.h"
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
#include "SoundMgr.h"



CEmployshop::CEmployshop()
	:m_iSelectIndex(0), m_bSelectEmploy(false), m_isDead(false), m_bSelectPortait(true), m_fPlusTime(0.f)
	, m_fAddTime(0.f)
{
}


CEmployshop::~CEmployshop()
{
	Release();
}


HRESULT	CEmployshop::Initialize(void)
{
	LoadObject();
	m_eRenderID = RENDER_TYPE_OBJECT;
	return S_OK;
}

int		CEmployshop::Update(void)
{
	if (m_isDead)
		return 1;

	SelectObject();

	//if (!m_bSelectPortait)
	//{
	//	m_fAddTime += GET_TIME * 3.f;
	//	if (m_fAddTime > m_fPlusTime)
	//	{
	//		m_fAddTime = 0.f;
	//		CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Player);
	//		m_bSelectPortait = true;
	//	}
	//}

	BuyEmploy();
	return 0;
}

void CEmployshop::Render(void)
{
	const TEX_INFO* pTileTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
			L"TownBack",
			5);

	if (pTileTex == NULL)
	{
		ERROR_MSG(L"백그라운드 이미지 X");
		return;
	}

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tInfo.matWorld = matScale;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	float fX = CENTER_POS(pTileTex->ImageInfo.Width);
	float fY = CENTER_POS(pTileTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTex->pTexture,
		NULL,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);

	////////////////////////////////////////////////////////////////////////////////////////////////////

	const TEX_INFO* pObjectTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
			L"TownBack",
			8);

	if (pObjectTex == NULL)
	{
		ERROR_MSG(L"백그라운드 이미지 X");
		return;
	}


	D3DXMatrixTranslation(&matTrans
		, 400.f
		, 300.f
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	float fX2 = CENTER_POS(pObjectTex->ImageInfo.Width);
	float fY2 = CENTER_POS(pObjectTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pObjectTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX2, fY2, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);


	////////////////////////////////////////////////////////////////////////////////////////////////////

	for (size_t i = 0; i < m_vecEmploy.size()-1; ++i)
	{
		const TEX_INFO* pEmployObjectTex =
			CTextureMgr::GetInstance()->GetTexture(m_vecEmploy[i]->wstrObjKey,
				m_vecEmploy[i]->wstrStateKey,
				m_vecEmploy[i]->byDrawID);

		if (pEmployObjectTex == NULL)
		{
			ERROR_MSG(L"백그라운드 이미지 X");
			return;
		}


		D3DXMatrixTranslation(&matTrans
			, m_vecEmploy[i] ->vPos.x
			, m_vecEmploy[i] ->vPos.y
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		float fX3 = CENTER_POS(pEmployObjectTex->ImageInfo.Width);
		float fY3 = CENTER_POS(pEmployObjectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->Draw(
			pEmployObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX3, fY3, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
			);
	}

	D3DXMatrixTranslation(&matTrans, m_vecEmploy[0]->vPos.x - 15, m_vecEmploy[0]->vPos.y - 5, 0.f);

	TCHAR		m_szShop[MIN_STR];

	wsprintf(m_szShop, L"판매");

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetCityFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		m_szShop,
		lstrlen(m_szShop),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 0, 0, 0)
	);

	D3DXMatrixTranslation(&matTrans, m_vecEmploy[1]->vPos.x - 15, m_vecEmploy[1]->vPos.y - 5, 0.f);

	TCHAR		m_szShop2[MIN_STR];

	wsprintf(m_szShop2, L"구매");

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetCityFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		m_szShop2,
		lstrlen(m_szShop2),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 0, 0, 0)
	);

	D3DXMatrixTranslation(&matTrans, m_vecEmploy[3]->vPos.x - 25, m_vecEmploy[3]->vPos.y - 5, 0.f);

	TCHAR		m_szShop3[MIN_STR];

	wsprintf(m_szShop3, L"나가기");

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetCityFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		m_szShop3,
		lstrlen(m_szShop3),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 0, 0, 0)
	);


	////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_bSelectEmploy)
	{
		int iMax = m_vecEmploy.size()-1;
		const TEX_INFO* pEmployObjectSelTex =
			CTextureMgr::GetInstance()->GetTexture(m_vecEmploy[iMax]->wstrObjKey,
				m_vecEmploy[iMax]->wstrStateKey,
				m_vecEmploy[iMax]->byDrawID);

		if (pEmployObjectSelTex == NULL)
		{
			ERROR_MSG(L"백그라운드 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, m_vecEmploy[iMax]->vPos.x
			, m_vecEmploy[iMax]->vPos.y
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		float fX4 = CENTER_POS(pEmployObjectSelTex->ImageInfo.Width);
		float fY4 = CENTER_POS(pEmployObjectSelTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->Draw(
			pEmployObjectSelTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX4, fY4, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

void	CEmployshop::Release(void)
{
	for (size_t i = 0; i < m_vecEmploy.size(); ++i)
	{
		safeDelete(m_vecEmploy[i]);
	}
	m_vecEmploy.clear();

	CSoundMgr::GetInstance()->StopAll();
}

void CEmployshop::LoadObject()
{
	OBJECT* pCityData = new OBJECT;
	ZeroMemory(pCityData, sizeof(OBJECT));
	pCityData->vPos = D3DXVECTOR3(400.f, 467.f, 0.f);
	pCityData->vSize = D3DXVECTOR3(78.f, 17.f, 0.f);
	lstrcpy(pCityData->wstrObjKey, L"UI");
	lstrcpy(pCityData->wstrStateKey, L"TownButton");
	pCityData->byDrawID = 15;
	pCityData->byOption = 1;  // 판매

	m_vecEmploy.push_back(pCityData);

	OBJECT* pCityData2 = new OBJECT;
	ZeroMemory(pCityData2, sizeof(OBJECT));
	pCityData2->vPos = D3DXVECTOR3(294.f, 467.f, 0.f);
	pCityData2->vSize = D3DXVECTOR3(78.f, 17.f, 0.f);
	lstrcpy(pCityData2->wstrObjKey, L"UI");
	lstrcpy(pCityData2->wstrStateKey, L"TownButton");
	pCityData2->byDrawID = 15;
	pCityData2->byOption = 0;  // 구매

	m_vecEmploy.push_back(pCityData2);

	OBJECT* pCityData3 = new OBJECT;
	ZeroMemory(pCityData3, sizeof(OBJECT));
	pCityData3->vPos = D3DXVECTOR3(500.f, 467.f, 0.f);
	pCityData3->vSize = D3DXVECTOR3(78.f, 17.f, 0.f);
	lstrcpy(pCityData3->wstrObjKey, L"UI");
	lstrcpy(pCityData3->wstrStateKey, L"TownButton");
	pCityData3->byDrawID = 15;
	pCityData3->byOption = -1;

	m_vecEmploy.push_back(pCityData3);

	OBJECT* pCityData4 = new OBJECT;
	ZeroMemory(pCityData4, sizeof(OBJECT));
	pCityData4->vPos = D3DXVECTOR3(400.f, 497.f, 0.f);
	pCityData4->vSize = D3DXVECTOR3(68.f, 21.f, 0.f);
	lstrcpy(pCityData4->wstrObjKey, L"UI");
	lstrcpy(pCityData4->wstrStateKey, L"TownButton");
	pCityData4->byDrawID = 7;
	pCityData4->byOption = 3;   // 나가기

	m_vecEmploy.push_back(pCityData4);

	OBJECT* pCityData5 = new OBJECT;
	ZeroMemory(pCityData5, sizeof(OBJECT));
	pCityData5->vPos = D3DXVECTOR3(185.f, 193.f, 0.f);
	pCityData5->vSize = D3DXVECTOR3(144.f, 82.f, 0.f);
	lstrcpy(pCityData5->wstrObjKey, L"Town");
	lstrcpy(pCityData5->wstrStateKey, L"GBarrack");
	pCityData5->byDrawID = 2;
	pCityData5->byOption = -1;

	m_vecEmploy.push_back(pCityData5);

	// 용병상점 용병데이터

	OBJECT* pCityItemData0 = new OBJECT;
	ZeroMemory(pCityItemData0, sizeof(OBJECT));
	pCityItemData0->vPos = D3DXVECTOR3(208.f, 295.f, 0.f);
	pCityItemData0->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData0->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData0->wstrStateKey, L"SoonbiYean");
	pCityItemData0->byDrawID = 0;
	pCityItemData0->byOption = -1;

	m_vecEmploy.push_back(pCityItemData0);

	OBJECT* pCityItemData1 = new OBJECT;
	ZeroMemory(pCityItemData1, sizeof(OBJECT));
	pCityItemData1->vPos = D3DXVECTOR3(303.f, 295.f, 0.f);
	pCityItemData1->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData1->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData1->wstrStateKey, L"LeeSoonsin");
	pCityItemData1->byDrawID = 0;
	pCityItemData1->byOption = -1;

	m_vecEmploy.push_back(pCityItemData1);

	OBJECT* pCityItemData2 = new OBJECT;
	ZeroMemory(pCityItemData2, sizeof(OBJECT));
	pCityItemData2->vPos = D3DXVECTOR3(398.f, 295.f, 0.f);
	pCityItemData2->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData2->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData2->wstrStateKey, L"Turtle");
	pCityItemData2->byDrawID = 0;
	pCityItemData2->byOption = -1;

	m_vecEmploy.push_back(pCityItemData2);

	OBJECT* pCityItemData3 = new OBJECT;
	ZeroMemory(pCityItemData3, sizeof(OBJECT));
	pCityItemData3->vPos = D3DXVECTOR3(493.f, 295.f, 0.f);
	pCityItemData3->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData3->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData3->wstrStateKey, L"Tiger");
	pCityItemData3->byDrawID = 0;
	pCityItemData3->byOption = -1;

	m_vecEmploy.push_back(pCityItemData3);

	OBJECT* pCityItemData4 = new OBJECT;
	ZeroMemory(pCityItemData4, sizeof(OBJECT));
	pCityItemData4->vPos = D3DXVECTOR3(588.f, 295.f, 0.f);
	pCityItemData4->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData4->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData4->wstrStateKey, L"FireBird");
	pCityItemData4->byDrawID = 0;
	pCityItemData4->byOption = -1;

	m_vecEmploy.push_back(pCityItemData4);

	OBJECT* pCityItemData5 = new OBJECT;
	ZeroMemory(pCityItemData5, sizeof(OBJECT));
	pCityItemData5->vPos = D3DXVECTOR3(208.f, 390.f, 0.f);
	pCityItemData5->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData5->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData5->wstrStateKey, L"Dragon");
	pCityItemData5->byDrawID = 0;
	pCityItemData5->byOption = -1;

	m_vecEmploy.push_back(pCityItemData5);

	OBJECT* pCityItemData6 = new OBJECT;
	ZeroMemory(pCityItemData6, sizeof(OBJECT));
	pCityItemData6->vPos = D3DXVECTOR3(303.f, 390.f, 0.f);
	pCityItemData6->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData6->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData6->wstrStateKey, L"Guangmok");
	pCityItemData6->byDrawID = 0;
	pCityItemData6->byOption = -1;

	m_vecEmploy.push_back(pCityItemData6);

	OBJECT* pCityItemData7 = new OBJECT;
	ZeroMemory(pCityItemData7, sizeof(OBJECT));
	pCityItemData7->vPos = D3DXVECTOR3(398.f, 390.f, 0.f);
	pCityItemData7->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData7->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData7->wstrStateKey, L"Damoon");
	pCityItemData7->byDrawID = 0;
	pCityItemData7->byOption = -1;

	m_vecEmploy.push_back(pCityItemData7);

	OBJECT* pCityItemData8 = new OBJECT;
	ZeroMemory(pCityItemData8, sizeof(OBJECT));
	pCityItemData8->vPos = D3DXVECTOR3(493.f, 390.f, 0.f);
	pCityItemData8->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData8->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData8->wstrStateKey, L"Jeungjang");
	pCityItemData8->byDrawID = 0;
	pCityItemData8->byOption = -1;

	m_vecEmploy.push_back(pCityItemData8);

	OBJECT* pCityItemData9 = new OBJECT;
	ZeroMemory(pCityItemData9, sizeof(OBJECT));
	pCityItemData9->vPos = D3DXVECTOR3(588.f, 390.f, 0.f);
	pCityItemData9->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pCityItemData9->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData9->wstrStateKey, L"Jiguk");
	pCityItemData9->byDrawID = 0;
	pCityItemData9->byOption = -1;

	m_vecEmploy.push_back(pCityItemData9);

	OBJECT* pCityItemData10 = new OBJECT;
	ZeroMemory(pCityItemData10, sizeof(OBJECT));
	pCityItemData10->vPos = D3DXVECTOR3(208.f, 295.f, 0.f);
	pCityItemData10->vSize = D3DXVECTOR3(86.f, 86.f, 0.f);
	lstrcpy(pCityItemData10->wstrObjKey, L"Portrait");
	lstrcpy(pCityItemData10->wstrStateKey, L"PortraitUI");
	pCityItemData10->byDrawID = 13;
	pCityItemData10->byOption = -1;

	m_vecEmploy.push_back(pCityItemData10);
}
void CEmployshop::SelectObject()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	int iIndex = GetCityIndex(vMousePos);

	if (iIndex != -1)
	{
		m_iSelectIndex = iIndex;
		if (m_vecEmploy[m_iSelectIndex]->byDrawID == 7)
			m_vecEmploy[m_iSelectIndex]->byDrawID = 8;
		else if (m_vecEmploy[m_iSelectIndex]->byDrawID == 15)
			m_vecEmploy[m_iSelectIndex]->byDrawID = 16;

		if (lstrcmp(m_vecEmploy[iIndex]->wstrObjKey, L"Portrait") == 0)
		{
			int iMax = m_vecEmploy.size() - 1;

			DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

			D3DXVECTOR3 vMousePos = CMouse::GetMousePos();
			if (KEYPRESS(dwKey, LBUTTON))
			{
				m_vecEmploy[iMax]->vPos = m_vecEmploy[m_iSelectIndex]->vPos;
				m_bSelectEmploy = true;

				//if (m_bSelectPortait)
				//{
					if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Player") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Player_select.wav", CSoundMgr::Channel_Unit0);
						m_fPlusTime = 6.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"LeeSoonsin") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Leesonnsin_select.wav", CSoundMgr::Channel_Unit1);
						m_fPlusTime = 7.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Jeungjang") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Juang_select.wav", CSoundMgr::Channel_Unit2);
						m_fPlusTime = 7.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Guangmok") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Guangmok_select.wav", CSoundMgr::Channel_Unit3);
						m_fPlusTime = 9.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"FireBird") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"FireBird_select.wav", CSoundMgr::Channel_Unit4);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Dragon") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Dragon_select.wav", CSoundMgr::Channel_Unit5);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Damoon") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Damoon_select.wav", CSoundMgr::Channel_Unit6);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"SoonbiYean") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Soon_select.wav", CSoundMgr::Channel_Unit7);
						m_fPlusTime = 6.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Tiger") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Tiger_select.wav", CSoundMgr::Channel_Unit8);
						m_fPlusTime = 9.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Turtle") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Turtle_select.wav", CSoundMgr::Channel_Unit9);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(m_vecEmploy[m_iSelectIndex]->wstrStateKey, L"Jiguk") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Jiguk_select.wav", CSoundMgr::Channel_Unit10);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
				//}
			}
		}
	}
	else
	{
		if (m_vecEmploy[m_iSelectIndex]->byDrawID == 8)
			m_vecEmploy[m_iSelectIndex]->byDrawID = 7;
		else if (m_vecEmploy[m_iSelectIndex]->byDrawID == 16)
			m_vecEmploy[m_iSelectIndex]->byDrawID = 15;
	}
}
bool CEmployshop::Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(pObject->vPos.x,
		pObject->vPos.y - (pObject->vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(pObject->vPos.x + (pObject->vSize.x / 2.f),
			pObject->vPos.y,
			0.f),

		D3DXVECTOR3(pObject->vPos.x,
			pObject->vPos.y + (pObject->vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(pObject->vPos.x - (pObject->vSize.x / 2.f),
			pObject->vPos.y,
			0.f)
	};


	//벡터의 뺄셈으로 방향벡터를 구하자.
	D3DXVECTOR3 vDirection[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] =
	{
		D3DXVECTOR3(vDirection[0].y,
		-vDirection[0].x,
			0.f),

		D3DXVECTOR3(vDirection[1].y,
			-vDirection[1].x,
			0.f),

		D3DXVECTOR3(vDirection[2].y,
			-vDirection[2].x,
			0.f),

		D3DXVECTOR3(vDirection[3].y,
			-vDirection[3].x,
			0.f)
	};


	//단위벡터로 만들어 주자.
	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = vPos - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if (fDotResult > 0.f)
			return false;
	}

	return true;
}
int CEmployshop::GetCityIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecEmploy.size(); ++i)
	{
		if (Picking(vPos, m_vecEmploy[i]))
		{
			return i;
		}
	}
	return -1;
}

void CEmployshop::BuyEmploy()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	int iIndex = GetCityIndex(vMousePos);

	if (iIndex != -1)
	{
		if (m_bSelectEmploy)
		{
			DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

			static bool SelectEmploy = FALSE;

			if (KEYPRESS(dwKey, LBUTTON))
			{
				if (SelectEmploy == FALSE)
				{
					SelectEmploy = TRUE;
					if (m_vecEmploy[iIndex]->byOption == 0)
					{
						int iMax = m_vecEmploy.size() - 1;
						for (size_t i = 0; i < m_vecEmploy.size() - 1; ++i)
						{
							if (m_vecEmploy[i]->vPos == m_vecEmploy[iMax]->vPos)
							{
								vector<OBJECT*>* pVector = CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait();
								for (size_t j = 0; j < CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait()->size(); ++j)
								{
									if (lstrcmp((*CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait())[j]->wstrStateKey, L"PortraitUI") == 0)
									{
										lstrcpy((*CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait())[j]->wstrStateKey, m_vecEmploy[i]->wstrStateKey);
										(*CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait())[j]->byDrawID = 2;
										(*CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait())[j]->byOption = 255;
										BuyChar(m_vecEmploy[i]->wstrStateKey);

										vector<CGameObject*>::iterator iter = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().begin();
										vector<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().end();
										
										++iter;
										int iCount = 0;
										D3DXVECTOR3 vUnit;
										for ( ; iter != iter_end ; ++iter)
										{
											if (iCount == 0)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(40.f , 0.f , 0.f);
											else if(iCount == 1)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(-40.f, 0.f, 0.f);
											else if (iCount == 2)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(0.f, 40.f, 0.f);
											else if (iCount == 3)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(0.f, -40.f, 0.f);
											else if (iCount == 4)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(-30.f, -30.f, 0.f);
											else if (iCount == 5)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(30.f, -30.f, 0.f);
											else if (iCount == 6)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(30.f, 30.f, 0.f);
											else if (iCount == 7)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(-30.f, -30.f, 0.f);
											else if (iCount == 8)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(50.f, 0.f, 0.f);
											else if (iCount == 9)
												vUnit = D3DXVECTOR3(100.f, 100.f, 0.f) - D3DXVECTOR3(-50.f, 0.f, 0.f);
											(*iter)->SetUnitPos(vUnit.x, vUnit.y);
											++iCount;
										}
										return;
									}
								}
							}
						}
					}
					else if (m_vecEmploy[iIndex]->byOption == 1)
					{
						bool SellUnit = true;
						vector<OBJECT*>* pVector = CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait();
						for (size_t i = 1; i < pVector->size(); ++i)
						{
							if (lstrcmp((*pVector)[i]->wstrStateKey, L"PortraitUI") != 0)
							{
								if ((*pVector)[i]->byDrawID == 1)
								{
									for (size_t j = i; j < pVector->size(); ++j)
									{
										if (lstrcmp((*pVector)[j]->wstrStateKey, L"PortraitUI") != 0)
										{
											if (j == 10)
											{
												lstrcpy((*pVector)[j]->wstrStateKey, L"PortraitUI");
												(*pVector)[j]->byOption = 160;
												(*pVector)[j]->byDrawID = 1;
												(*pVector)[j]->vPos.x = 50.f;
												(*pVector)[j]->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
											}
											else
											{
												lstrcpy((*pVector)[j]->wstrStateKey, (*pVector)[j + 1]->wstrStateKey);
												if (SellUnit)
												{
													SellChar(j);
													SellUnit = false;
												}
											}
										}
										else
										{
											lstrcpy((*pVector)[j-1]->wstrStateKey, L"PortraitUI");
											(*pVector)[j-1]->byOption = 160;
											(*pVector)[j-1]->byDrawID = 1;
											(*pVector)[j-1]->vPos.x = 50.f;
											(*pVector)[j-1]->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
										}
									}
								}
							}
						}
					}
					else if (m_vecEmploy[iIndex]->byOption == 3)
					{
						CObjMgr::GetInstance()->FindGameObject<CCityBackGround>()->SetView(false);
						m_isDead = true;
					}
				}
			}
			else
			{
				SelectEmploy = FALSE;
			}
		}
	}
}

void CEmployshop::BuyChar(TCHAR  wstrStateKey[MIDDLE_STR])
{
	if (lstrcmp(wstrStateKey, L"Damoon") == 0)
	{
		CGameObject* pObject = new CDamoon;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Dragon") == 0)
	{
		CGameObject* pObject = new CDragon;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"FireBird") == 0)
	{
		CGameObject* pObject = new CFireBird;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Guangmok") == 0)
	{
		CGameObject* pObject = new CGuangmok;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Jeungjang") == 0)
	{
		CGameObject* pObject = new CJeungjang;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Jiguk") == 0)
	{
		CGameObject* pObject = new CJiguk;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"LeeSoonsin") == 0)
	{
		CGameObject* pObject = new CLeeSoonsin;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"SoonbiYean") == 0)
	{
		CGameObject* pObject = new CSunbiyean;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Tiger") == 0)
	{
		CGameObject* pObject = new CTiger;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
	else if (lstrcmp(wstrStateKey, L"Turtle") == 0)
	{
		CGameObject* pObject = new CTurtle;
		pObject->Initialize();
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().push_back(pObject);
		return;
	}
}


void CEmployshop::SellChar(int iIndex)
{
	vector<CGameObject*>::iterator iter = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().begin();
	vector<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().end();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().erase(iter);
}