#include "stdafx.h"
#include "BattleBackGround.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Player.h"
#include "Mouse.h"
#include "TimeMgr.h"


CBattleBackGround::CBattleBackGround()
{
}


CBattleBackGround::~CBattleBackGround()
{
	Release();
}
HRESULT	CBattleBackGround::Initialize(void)
{
	LoadTile();
	LoadObject();

	m_BattleScroll = D3DXVECTOR3(0.f, 0.f, 0.f);

	vector<CGameObject*>::iterator iter = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().begin();
	vector<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEmploy().end();
	
	dynamic_cast<CPlayer*>((*iter))->SetPlayerView(true);
	dynamic_cast<CPlayer*>((*iter))->SetPos(100.f, 200.f);

	++iter;

	for (; iter != iter_end; ++iter)
	{
		CObjMgr::GetInstance()->GetList()[OBJ_TYPE_UNIT].push_back((*iter));
	}

	m_ptTileRender.x = WINCX / TILECX + 3;
	m_ptTileRender.y = WINCY / (TILECY / 2) + 3;

	m_fSpeed = 300.f;

	return S_OK;
}
int	CBattleBackGround::Update(void)
{
	MouseScrollCheck();
	return 0;
}
void CBattleBackGround::Render(void)
{
	D3DXMATRIX matWorld, matTrans, matScale;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	for (int i = 0; i < m_ptTileRender.y; ++i)
	{
		for (int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = int(-m_BattleScroll.x / TILECX);
			int iCullY = int((-m_BattleScroll.y) / (TILECY / 2.f));

			int iIndex = (i + iCullY) * TILEX + (j + iCullX);

			if (iIndex < 0 || TILEX * TILEY <= iIndex)
				break;

			const TEX_INFO* pTileTex =
				CTextureMgr::GetInstance()->GetTexture(L"Texture",
					L"Tile",
					m_vecTile[iIndex]->byDrawID);


			if (pTileTex == NULL)
			{
				ERROR_MSG(L"타일 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, int(m_vecTile[iIndex]->vPos.x + m_BattleScroll.x)
				, int(m_vecTile[iIndex]->vPos.y + m_BattleScroll.y)
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
		}
	}
	for (size_t i = 0; i < m_vecCity.size(); ++i)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture(L"Back", L"Object", m_vecCity[i]->byDrawID);


		if (pObjectTex == NULL)
		{
			ERROR_MSG(L"마을 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, m_vecCity[i]->vPos.x + m_BattleScroll.x
			, m_vecCity[i]->vPos.y + m_BattleScroll.y
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}
void CBattleBackGround::Release(void)
{
	vector<TILE*>::iterator iter = m_vecTile.begin();

	for (; iter != m_vecTile.end(); ++iter)
	{
		safeDelete((*iter));
	}
	m_vecTile.clear();

}
void CBattleBackGround::LoadTile()
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
void CBattleBackGround::LoadObject()
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


	while (true)
	{
		INFO* pCityData = new INFO;

		ZeroMemory(pCityData, sizeof(INFO));

		ReadFile(hFile, pCityData, sizeof(INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			safeDelete(pCityData);
			break;
		}
		m_vecCity.push_back(pCityData);
	}

	CloseHandle(hFile);
}

void CBattleBackGround::MouseScrollCheck()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	float fTime = CTimeMgr::GetInstance()->GetTime();

	if (200 > vMousePos.x)
		m_BattleScroll.x += fTime * m_fSpeed;

	if (WINCX - 200 < vMousePos.x)
		m_BattleScroll.x -= fTime * m_fSpeed;
	if (200 > vMousePos.y)
		m_BattleScroll.y += fTime * m_fSpeed;

	if (WINCY - 200 < vMousePos.y)
		m_BattleScroll.y -= fTime * m_fSpeed;

	if (0 <= m_BattleScroll.x)
		m_BattleScroll.x = 0.f;
	if (0 <= m_BattleScroll.y)
		m_BattleScroll.y = 0.f;

	if (WINCX - (TILECX * TILEX) >= m_BattleScroll.x)
		m_BattleScroll.x = WINCX - (TILECX * TILEX);

	if (WINCY - ((TILECY / 2.f) * TILEY) >= m_BattleScroll.y)
		m_BattleScroll.y = WINCY - ((TILECY / 2.f) * TILEY);
}