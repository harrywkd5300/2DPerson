#include "stdafx.h"
#include "Shop.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "CityBackGround.h"
#include "Player.h"
#include "SceneMgr.h"

CShop::CShop()
	:m_isDead(false), m_bSelect(false)
{
}


CShop::~CShop()
{
	Release();
}

HRESULT	CShop::Initialize(void)
{
	LoadObject();
	m_eRenderID = RENDER_TYPE_OBJECT;
	return S_OK;
}

int	CShop::Update(void)
{
	if (m_isDead)
		return 1;
	Keyinput();
	return 0;
}

void CShop::Render(void)
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

	/////////////////////////////////////////////////////////////////////////////////

	const TEX_INFO* pObjectTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
			L"TownBack",
			7);

	if (pObjectTex == NULL)
	{
		ERROR_MSG(L"백그라운드 이미지 X");
		return;
	}


	D3DXMatrixTranslation(&matTrans
		, 280.f
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

	//////////////////////////////////////////////////////////////
	for (size_t i = 0; i < m_vecItem.size()-1 ; ++i)
	{
		const TEX_INFO* pEmployObjectTex =
			CTextureMgr::GetInstance()->GetTexture(m_vecItem[i]->wstrObjKey,
				m_vecItem[i]->wstrStateKey,
				m_vecItem[i]->byDrawID);

		if (pEmployObjectTex == NULL)
		{
			ERROR_MSG(L"상점오브젝트 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, m_vecItem[i]->vPos.x
			, m_vecItem[i]->vPos.y
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

	//D3DXVECTOR3 vBtPos = m_vecItem[0]->vPos;

	D3DXMatrixTranslation(&matTrans, m_vecItem[0]->vPos.x - 25, m_vecItem[0]->vPos.y - 5, 0);

	TCHAR		m_szShop[MIN_STR];

	wsprintf(m_szShop, L"나가기");

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetCityFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		m_szShop,
		lstrlen(m_szShop),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 0, 0, 0)
	);



	if (m_bSelect)
	{
		int iMax = m_vecItem.size() - 1;
		const TEX_INFO* pEmployObjectSelTex =
			CTextureMgr::GetInstance()->GetTexture(m_vecItem[iMax]->wstrObjKey,
				m_vecItem[iMax]->wstrStateKey,
				m_vecItem[iMax]->byDrawID);

		if (pEmployObjectSelTex == NULL)
		{
			ERROR_MSG(L"백그라운드 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, m_vecItem[iMax]->vPos.x
			, m_vecItem[iMax]->vPos.y
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

void CShop::Release(void)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		safeDelete(m_vecItem[i]);
	}
	m_vecItem.clear();
}
void CShop::Keyinput()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	//vMousePos -= m_vScroll;

	int iIndex = ShopIndex(vMousePos);

	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	static bool SelectShop = FALSE;

	if (Picking(vMousePos, m_vecItem[0]))
		m_vecItem[0]->byDrawID = 8;
	else 
		m_vecItem[0]->byDrawID = 7;

	if (iIndex != -1)
	{
		if (lstrcmp(m_vecItem[iIndex]->wstrStateKey, L"TownButton") == 0)
		{
			if (KEYPRESS(dwKey, LBUTTON))
			{
				if (SelectShop == FALSE)
				{
					SelectShop = TRUE;
					CObjMgr::GetInstance()->FindGameObject<CCityBackGround>()->SetItemView(false);
					m_isDead = true;
				}
			}
			else
			{
				SelectShop = FALSE;
			}
		}

		if (lstrcmp(m_vecItem[iIndex]->wstrObjKey, L"Item") == 0)
		{
			int iMax = m_vecItem.size() - 1;

			if (KEYPRESS(dwKey, LBUTTON))
			{
				m_vecItem[iMax]->vPos = m_vecItem[iIndex]->vPos;
				m_bSelect = true;
				if (SelectShop == FALSE)
				{
					SelectShop = TRUE;
				}
			}
			else if (KEYPRESS(dwKey, RBUTTON))
			{
				if (SelectShop == FALSE)
				{
					SelectShop = TRUE;
					if(m_vecItem[iMax]->vPos == m_vecItem[iIndex]->vPos)
						BuyItem(m_vecItem[iIndex]);
				}
			}
			else
			{
				SelectShop = FALSE;
			}
		}
	}
}

int CShop::ShopIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (Picking(vPos, m_vecItem[i]))
		{
			return i;
		}
	}
	return -1;
}
bool CShop::Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(pObject->vPos.x,
		pObject->vPos.y - (pObject->vSize.y),
		0.f),

		D3DXVECTOR3(pObject->vPos.x + (pObject->vSize.x),
		pObject->vPos.y,
		0.f),

		D3DXVECTOR3(pObject->vPos.x,
		pObject->vPos.y + (pObject->vSize.y),
		0.f),

		D3DXVECTOR3(pObject->vPos.x - (pObject->vSize.x),
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



void CShop::LoadObject()
{
	//상점창 종료버튼
	OBJECT* pCityData = new OBJECT;
	ZeroMemory(pCityData, sizeof(OBJECT));
	pCityData->vPos = D3DXVECTOR3(280.f, 498.f, 0.f);
	pCityData->vSize = D3DXVECTOR3(68.f, 21.f, 0.f);
	lstrcpy(pCityData->wstrObjKey, L"UI");
	lstrcpy(pCityData->wstrStateKey, L"TownButton");
	pCityData->byDrawID = 7;
	pCityData->byOption = -1;

	m_vecItem.push_back(pCityData);

	//주인아저씨 이미지
	OBJECT* pItemData2 = new OBJECT;
	ZeroMemory(pItemData2, sizeof(OBJECT));
	pItemData2->vPos = D3DXVECTOR3(160.f, 165.f, 0.f);
	pItemData2->vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(pItemData2->wstrObjKey, L"Town");
	lstrcpy(pItemData2->wstrStateKey, L"WStore");
	pItemData2->byDrawID = 2;
	pItemData2->byOption = -1;

	m_vecItem.push_back(pItemData2);

	OBJECT* pItemData3 = new OBJECT;
	ZeroMemory(pItemData3, sizeof(OBJECT));
	pItemData3->vPos = D3DXVECTOR3(152.f, 250.f, 0.f);
	pItemData3->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData3->wstrObjKey, L"Item");
	lstrcpy(pItemData3->wstrStateKey, L"Gumangdo");
	pItemData3->byDrawID = 0;
	pItemData3->byOption = 0;

	m_vecItem.push_back(pItemData3);

	OBJECT* pItemData4 = new OBJECT;
	ZeroMemory(pItemData4, sizeof(OBJECT));
	pItemData4->vPos = D3DXVECTOR3(214.f, 250.f, 0.f);
	pItemData4->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData4->wstrObjKey, L"Item");
	lstrcpy(pItemData4->wstrStateKey, L"ThunderGun");
	pItemData4->byDrawID = 0;
	pItemData4->byOption = 0;

	m_vecItem.push_back(pItemData4);

	OBJECT* pItemData5 = new OBJECT;
	ZeroMemory(pItemData5, sizeof(OBJECT));
	pItemData5->vPos = D3DXVECTOR3(280.f, 250.f, 0.f);
	pItemData5->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData5->wstrObjKey, L"Item");
	lstrcpy(pItemData5->wstrStateKey, L"Dragon_Head");
	pItemData5->byDrawID = 0;
	pItemData5->byOption = 2;

	m_vecItem.push_back(pItemData5);

	OBJECT* pItemData6 = new OBJECT;
	ZeroMemory(pItemData6, sizeof(OBJECT));
	pItemData6->vPos = D3DXVECTOR3(344.f, 250.f, 0.f);
	pItemData6->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData6->wstrObjKey, L"Item");
	lstrcpy(pItemData6->wstrStateKey, L"Dragon_Body");
	pItemData6->byDrawID = 0;
	pItemData6->byOption = 1;

	m_vecItem.push_back(pItemData6);

	OBJECT* pItemData7 = new OBJECT;
	ZeroMemory(pItemData7, sizeof(OBJECT));
	pItemData7->vPos = D3DXVECTOR3(408.f, 250.f, 0.f);
	pItemData7->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData7->wstrObjKey, L"Item");
	lstrcpy(pItemData7->wstrStateKey, L"FrowerRing");
	pItemData7->byDrawID = 0;
	pItemData7->byOption = 3;

	m_vecItem.push_back(pItemData7);

	OBJECT* pItemData8 = new OBJECT;
	ZeroMemory(pItemData8, sizeof(OBJECT));
	pItemData8->vPos = D3DXVECTOR3(152.f, 314.f, 0.f);
	pItemData8->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData8->wstrObjKey, L"Item");
	lstrcpy(pItemData8->wstrStateKey, L"Beauti_Head");
	pItemData8->byDrawID = 0;
	pItemData8->byOption = 2;

	m_vecItem.push_back(pItemData8);

	OBJECT* pItemData9 = new OBJECT;
	ZeroMemory(pItemData9, sizeof(OBJECT));
	pItemData9->vPos = D3DXVECTOR3(216.f, 314.f, 0.f);
	pItemData9->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData9->wstrObjKey, L"Item");
	lstrcpy(pItemData9->wstrStateKey, L"Beauti_Body");
	pItemData9->byDrawID = 0;
	pItemData9->byOption = 1;

	m_vecItem.push_back(pItemData9);

	OBJECT* pItemData10 = new OBJECT;
	ZeroMemory(pItemData10, sizeof(OBJECT));
	pItemData10->vPos = D3DXVECTOR3(280.f, 314.f, 0.f);
	pItemData10->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData10->wstrObjKey, L"Item");
	lstrcpy(pItemData10->wstrStateKey, L"Beauti_Body");
	pItemData10->byDrawID = 0;
	pItemData10->byOption = 1;

	m_vecItem.push_back(pItemData10);

	OBJECT* pItemData11 = new OBJECT;
	ZeroMemory(pItemData11, sizeof(OBJECT));
	pItemData11->vPos = D3DXVECTOR3(344.f, 314.f, 0.f);
	pItemData11->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData11->wstrObjKey, L"Item");
	lstrcpy(pItemData11->wstrStateKey, L"Beauti_Body");
	pItemData11->byDrawID = 0;
	pItemData11->byOption = 1;

	m_vecItem.push_back(pItemData11);

	OBJECT* pItemData12 = new OBJECT;
	ZeroMemory(pItemData12, sizeof(OBJECT));
	pItemData12->vPos = D3DXVECTOR3(408.f, 314.f, 0.f);
	pItemData12->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData12->wstrObjKey, L"Item");
	lstrcpy(pItemData12->wstrStateKey, L"Drug");
	pItemData12->byDrawID = 0;
	pItemData12->byOption = 4;

	m_vecItem.push_back(pItemData12);

	OBJECT* pItemData13 = new OBJECT;
	ZeroMemory(pItemData13, sizeof(OBJECT));
	pItemData13->vPos = D3DXVECTOR3(152.f, 380.f, 0.f);
	pItemData13->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData13->wstrObjKey, L"Item");
	lstrcpy(pItemData13->wstrStateKey, L"Drug");
	pItemData13->byDrawID = 5;
	pItemData13->byOption = 4;

	m_vecItem.push_back(pItemData13);


	OBJECT* pItemData99 = new OBJECT;
	ZeroMemory(pItemData99, sizeof(OBJECT));
	pItemData99->vPos = D3DXVECTOR3(152.f, 250.f, 0.f);
	pItemData99->vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	lstrcpy(pItemData99->wstrObjKey, L"Portrait");
	lstrcpy(pItemData99->wstrStateKey, L"PortraitUI");
	pItemData99->byDrawID = 12;
	pItemData99->byOption = -1;

	m_vecItem.push_back(pItemData99);
}

void CShop::BuyItem(OBJECT* pObject)
{
	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->BuyItem(pObject->wstrStateKey, pObject->byOption);
}