#include "stdafx.h"
#include "CityBackGround.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Employshop.h"
#include "SceneMgr.h"
#include "Shop.h"
#include "Player.h"

CCityBackGround::CCityBackGround()
	:m_iSelectIndex(0), m_bOpenShop(false), m_bOpenItemShop(false)
{

}

CCityBackGround::~CCityBackGround()
{
	Release();
}

HRESULT	CCityBackGround::Initialize(void)
{
	LoadCity();

	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetPlayerView(false);
	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetNotMove(false);

	m_eRenderID = RENDER_TYPE_BACKGROUND;
	return S_OK;
}
int	CCityBackGround::Update(void)
{
	if (!m_bOpenShop && !m_bOpenItemShop)
	{
		SelectObject();
		KeyInput();
	}
	return 0;
}

void CCityBackGround::Render(void)
{
	if (!m_bOpenShop && !m_bOpenItemShop)
	{
		const TEX_INFO* pTileTex =
			CTextureMgr::GetInstance()->GetTexture(L"UI",
				L"TownBack",
				13);

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

		for (size_t i = 0; i < m_vecCity.size(); ++i)
		{
			const TEX_INFO* pObjectTex =
				CTextureMgr::GetInstance()->GetTexture(m_vecCity[i]->wstrObjKey,
					m_vecCity[i]->wstrStateKey,
					m_vecCity[i]->byDrawID);


			if (pObjectTex == NULL)
			{
				ERROR_MSG(L"마을 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, m_vecCity[i]->vPos.x
				, m_vecCity[i]->vPos.y
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

	//TCHAR szIndexText[MIDDLE_STR] = L"";

	//wsprintf(szIndexText, L"나가기");
	//CDevice::GetInstance()->GetCityFont()->DrawTextW(
	//	CDevice::GetInstance()->GetSprite(),
	//	szIndexText,
	//	lstrlen(szIndexText),
	//	NULL,
	//	DT_CENTER,
	//	D3DCOLOR_ARGB(255, 0, 0, 0));

}

void CCityBackGround::Release(void)
{
	for (size_t i=0 ; i<m_vecCity.size() ; ++i)
	{
		safeDelete(m_vecCity[i]);
	}
	m_vecCity.clear();
}

void CCityBackGround::LoadCity()
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/CityObject.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
	);


	while (true)
	{
		OBJECT* pCityData = new OBJECT;

		ZeroMemory(pCityData, sizeof(OBJECT));

		ReadFile(hFile, pCityData, sizeof(OBJECT), &dwByte, NULL);

		if (dwByte == 0)
		{
			safeDelete(pCityData);
			break;
		}

		pCityData->vSize.x *= 1.5;
		pCityData->vSize.y *= 1.5;

		m_vecCity.push_back(pCityData);
	}

	m_vecCity[3]->vPos.x += 50.f;

	OBJECT* pCityData = new OBJECT;
	ZeroMemory(pCityData, sizeof(OBJECT));
	pCityData->vPos = m_vecCity[3]->vPos;
	pCityData->vSize = D3DXVECTOR3(90.f, 24.f , 0.f);
	lstrcpy(pCityData->wstrObjKey, L"UI");
	lstrcpy(pCityData->wstrStateKey, L"TownButton");
	pCityData->byDrawID = 0;

	m_vecCity.push_back(pCityData);

	CloseHandle(hFile);
}
void CCityBackGround::KeyInput()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	if (KEYPRESS(dwKey, LBUTTON))
	{
		int iIndex = GetCityIndex(vMousePos);
		if (iIndex != -1)
		{
			if (lstrcmp(m_vecCity[iIndex]->wstrStateKey, L"GBarrack")==0)
			{
				CObjMgr::GetInstance()->AddGameObject(
					CFactory<CEmployshop>::CreateGameObject(),
					OBJ_TYPE_UI);

				m_bOpenShop = true;
			}
			else if (lstrcmp(m_vecCity[iIndex]->wstrStateKey, L"WStore") == 0)
			{
				CObjMgr::GetInstance()->AddGameObject(
					CFactory<CShop>::CreateGameObject(),
					OBJ_TYPE_UI);

				m_bOpenItemShop = true;
			}
			if (lstrcmp(m_vecCity[iIndex]->wstrStateKey, L"TownButton") == 0 || lstrcmp(m_vecCity[iIndex]->wstrStateKey, L"TownBack") == 0)
			{
				for (int i = 0; i < 10; ++i)
				{
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i) != NULL)
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i)->SetMovePos(D3DXVECTOR3(300.f, 0.f, 0.f));
				}
				for (int i = 0; i < 8; ++i)
				{
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) != NULL)
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i)->SetMovePos(D3DXVECTOR3(300.f, 0.f, 0.f));
				}
				CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);
			}
		}
	}
}


bool CCityBackGround::Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject)
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

int CCityBackGround::GetCityIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecCity.size(); ++i)
	{
		if (Picking(vPos, m_vecCity[i]))
		{
			return i;
		}
	}
	return -1;
}


void CCityBackGround::SelectObject()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	int iIndex = GetCityIndex(vMousePos);
	if (iIndex != -1)
	{
		m_iSelectIndex = iIndex;

		if(m_iSelectIndex == 3)
			m_vecCity[m_iSelectIndex+1]->byDrawID = 1;

		if (m_vecCity[m_iSelectIndex]->byDrawID == 0)
			m_vecCity[m_iSelectIndex]->byDrawID = 1;
	}
	else
	{
		if (m_vecCity[m_iSelectIndex]->byDrawID == 1)
			m_vecCity[m_iSelectIndex]->byDrawID = 0;

		if (m_iSelectIndex == 3)
			m_vecCity[m_iSelectIndex + 1]->byDrawID = 0;
	}
}