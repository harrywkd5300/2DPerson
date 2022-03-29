#include "StdAfx.h"
#include "BackGround.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
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

CBackGround::CBackGround(void)
	:m_TILEMAP(false)
{
	
}

CBackGround::~CBackGround(void)
{
	Release();
}
HRESULT	CBackGround::Initialize(void)
{
	m_ptTileRender.x = WINCX / TILECX + 3;
	m_ptTileRender.y = WINCY / (TILECY / 2) + 3;
	//m_hdc = GetDC(g_hwnd);

	LoadTile();
	
	m_fSpeed = 200.f;
	if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	{
		vector<CGameObject*>::iterator iter		= CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().begin();
		vector<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().end();

		int iA = 0;
		(*iter) = (*CObjMgr::GetInstance()->GetList(OBJ_TYPE_PLAYER).begin());

		for ( ; iter != iter_end ; ++iter)
		{
			CObjMgr::GetInstance()->AddGameObject((*iter), OBJ_TYPE_UNIT);

			if (iA == 0)
			{
				iA = 1;
				CObjMgr::GetInstance()->GetList(OBJ_TYPE_PLAYER).pop_front();
				dynamic_cast<CPlayer*>((*iter))->SetPlayerView(true);
				dynamic_cast<CPlayer*>((*iter))->SetPos(300.f, 300.f);
				dynamic_cast<CPlayer*>((*iter))->SetPlayerPos(300.f, 300.f);
				CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetSceneChange(true);
			}
			else
			{
				if(lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"LeeSunsin") == 0)
					dynamic_cast<CLeeSoonsin*>((*iter))->Initialize();
				else if(lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"SunbiYean") == 0)
					dynamic_cast<CSunbiyean*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Dragon") == 0)
					dynamic_cast<CDragon*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Tiger") == 0)
					dynamic_cast<CTiger*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"FireBird") == 0)
					dynamic_cast<CFireBird*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Turtle") == 0)
					dynamic_cast<CTurtle*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Jiguk") == 0)
					dynamic_cast<CJiguk*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Damoon") == 0)
					dynamic_cast<CDamoon*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Guangmok") == 0)
					dynamic_cast<CGuangmok*>((*iter))->Initialize();
				else if (lstrcmp((*iter)->GetUnitInfo()->wstrObjKey, L"Jeungjang") == 0)
					dynamic_cast<CJeungjang*>((*iter))->Initialize();
			}
		}
	}

	m_eRenderID = RENDER_TYPE_BACKGROUND;
	return S_OK;
}
int	CBackGround::Update(void)
{
	MouseScrollCheck();
	return 0;
}
void CBackGround::Render(void)
{
	D3DXMATRIX matWorld, matTrans, matScale;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	for(int i = 0; i < m_ptTileRender.y ; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x ; ++j)
		{
			int iCullX = int(-m_vScroll.x / TILECX);
			int iCullY = int((-m_vScroll.y) / (TILECY / 2.f));

			int iIndex = (i+iCullY) * TILEX + (j + iCullX);

			if(iIndex < 0 || TILEX * TILEY <= iIndex)
				break;

			const TEX_INFO* pTileTex = 
				CTextureMgr::GetInstance()->GetTexture(L"Texture",
				L"Tile",
				m_vecTile[iIndex]->byDrawID);


			if(pTileTex == NULL)
			{
				ERROR_MSG(L"타일 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, float(int(m_vecTile[iIndex]->vPos.x-0.5f + m_vScroll.x))
				, float(int(m_vecTile[iIndex]->vPos.y-0.5f + m_vScroll.y))
				, 0.f);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			
			float fX = CENTER_POS(pTileTex->ImageInfo.Width);
			float fY = CENTER_POS(pTileTex->ImageInfo.Height);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL, 
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
				);

			if (m_TILEMAP)
			{
				const TEX_INFO* pBTileTex =
					CTextureMgr::GetInstance()->GetTexture(L"Texture1",
						L"TILEMAP",
						m_vecTile[iIndex]->byOption);

				D3DXMatrixTranslation(&matTrans
					, float(int(m_vecTile[iIndex]->vPos.x - 0.5f + m_vScroll.x))
					, float(int(m_vecTile[iIndex]->vPos.y - 0.5f + m_vScroll.y))
					, 0.f);

				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

				float fX = CENTER_POS(pBTileTex->ImageInfo.Width);
				float fY = CENTER_POS(pBTileTex->ImageInfo.Height);

				matWorld = matScale * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				CDevice::GetInstance()->GetSprite()->Draw(
					pBTileTex->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255)
				);
			}
		}
	}
}

void CBackGround::Release(void)
{
	vector<TILE*>::iterator iter = m_vecTile.begin();

	for(; iter != m_vecTile.end() ; ++iter)
	{
		safeDelete((*iter));
	}
	m_vecTile.clear();

	if (!CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).empty())
	{
		vector<CGameObject*>::iterator iter = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().begin();
		(*iter) = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).front();
		CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).pop_front();
		dynamic_cast<CPlayer*>((*iter))->SetPlayerView(true);

		CObjMgr::GetInstance()->ReleaseObj(OBJ_TYPE_UNIT);
	}
}
void CBackGround::LoadTile()
{
	if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
	{
		DWORD dwByte = 0;

		HANDLE hFile = CreateFile(
			L"../Data/StageTile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
			NULL
		);

		Release();

		while (true)
		{
			TILE* pTileData = new TILE;

			ZeroMemory(pTileData, sizeof(TILE));

			ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

			if (dwByte == 0)
			{
				safeDelete(pTileData);
				break;
			}

			m_vecTile.push_back(pTileData);
		}
		CloseHandle(hFile);
	}
	else if(CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	{
		DWORD dwByte = 0;

		HANDLE hFile = CreateFile(
			L"../Data/BattleTile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
			NULL
		);


		while (true)
		{
			TILE* pTileData = new TILE;

			ZeroMemory(pTileData, sizeof(TILE));

			ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

			if (dwByte == 0)
			{
				safeDelete(pTileData);
				break;
			}
			m_vecTile.push_back(pTileData);
		}

		CloseHandle(hFile);
	}
}
void CBackGround::MouseScrollCheck()
{
	D3DXVECTOR3 vPlayerPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos;

	float fTime = CTimeMgr::GetInstance()->GetTime();

	if (200 > vPlayerPos.x + m_vScroll.x)
		m_vScroll.x += fTime * m_fSpeed;

	if (WINCX - 200 < vPlayerPos.x + m_vScroll.x)
		m_vScroll.x -= fTime * m_fSpeed;
	if (200 > vPlayerPos.y + m_vScroll.y)
		m_vScroll.y += fTime * m_fSpeed;

	if (WINCY - 200 < vPlayerPos.y + m_vScroll.y)
		m_vScroll.y -= fTime * m_fSpeed;

	if (0 <= m_vScroll.x)
		m_vScroll.x = 0.f;
	if (0 <= m_vScroll.y)
		m_vScroll.y = 0.f;

	if (WINCX - (TILECX * TILEX) >= m_vScroll.x)
		m_vScroll.x = WINCX - (TILECX * TILEX);

	if (WINCY - ((TILECY / 2.f) * TILEY) >= m_vScroll.y)
		m_vScroll.y = WINCY - ((TILECY / 2.f) * TILEY);



	//if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
	//{
	//	D3DXVECTOR3 vPlayerPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos;

	//	float fTime = CTimeMgr::GetInstance()->GetTime();

	//	if (200 > vPlayerPos.x + m_vScroll.x)
	//		m_vScroll.x += fTime * m_fSpeed;

	//	if (WINCX - 200 < vPlayerPos.x + m_vScroll.x)
	//		m_vScroll.x -= fTime * m_fSpeed;
	//	if (200 > vPlayerPos.y + m_vScroll.y)
	//		m_vScroll.y += fTime * m_fSpeed;

	//	if (WINCY - 200 < vPlayerPos.y + m_vScroll.y)
	//		m_vScroll.y -= fTime * m_fSpeed;

	//	if (0 <= m_vScroll.x)
	//		m_vScroll.x = 0.f;
	//	if (0 <= m_vScroll.y)
	//		m_vScroll.y = 0.f;

	//	if (WINCX - (TILECX * TILEX) >= m_vScroll.x)
	//		m_vScroll.x = WINCX - (TILECX * TILEX);

	//	if (WINCY - ((TILECY / 2.f) * TILEY) >= m_vScroll.y)
	//		m_vScroll.y = WINCY - ((TILECY / 2.f) * TILEY);
	//}
	//else if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	//{
	///*	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();



	//	float fTime = CTimeMgr::GetInstance()->GetTime();

	//	if (200 > vMousePos.x)
	//		m_vScroll.x += fTime * m_fSpeed;

	//	if (WINCX - 200 < vMousePos.x)
	//		m_vScroll.x -= fTime * m_fSpeed;
	//	if (200 > vMousePos.y)
	//		m_vScroll.y += fTime * m_fSpeed;

	//	if (WINCY - 200 < vMousePos.y)
	//		m_vScroll.y -= fTime * m_fSpeed;

	//	if (0 <= m_vScroll.x)
	//		m_vScroll.x = 0.f;
	//	if (0 <= m_vScroll.y)
	//		m_vScroll.y = 0.f;

	//	if (WINCX - (TILECX * TILEX) >= m_vScroll.x)
	//		m_vScroll.x = WINCX - (TILECX * TILEX);

	//	if (WINCY - ((TILECY / 2.f) * TILEY) >= m_vScroll.y)
	//		m_vScroll.y = WINCY - ((TILECY / 2.f) * TILEY);*/
	//}
}