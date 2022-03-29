#include "stdafx.h"
#include "Inventory.h"
#include "Device.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Portrait.h"
#include "Item.h"
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
#include "CityBackGround.h"
#include "SceneMgr.h"



CInventory::CInventory()
	:m_bClose(false), m_bClickItem(false), m_iClickIndex(0), m_bHaveItem(false), m_iSelectIndex(0)
{
}


CInventory::~CInventory()
{
	Release();
}

HRESULT	CInventory::Initialize(void)
{
	if (CSceneMgr::GetInstance()->GetScene() == SCENE_CITY)
	{
		m_tUnit.vPos = D3DXVECTOR3(620.f, 300.f, 0.f);
		m_tUnit.vSize = D3DXVECTOR3(336.f, 418.f, 0.f);
		lstrcpy(m_tUnit.wstrObjKey, L"UI");
		lstrcpy(m_tUnit.wstrStateKey, L"CharInfo");
		m_tUnit.byDrawID = 0;
	}
	else
	{
		m_tUnit.vPos = D3DXVECTOR3(320.f, 300.f, 0.f);
		m_tUnit.vSize = D3DXVECTOR3(336.f, 418.f, 0.f);
		lstrcpy(m_tUnit.wstrObjKey, L"UI");
		lstrcpy(m_tUnit.wstrStateKey, L"CharInfo");
		m_tUnit.byDrawID = 0;
	}

	//m_hdc = GetDC(g_hwnd);

	m_ClickData.vPos = D3DXVECTOR3(m_tUnit.vPos.x + 5.f, m_tUnit.vPos.y + 195.f, 0.f);
	m_ClickData.vSize = D3DXVECTOR3(68.f, 21.f, 0.f);
	lstrcpy(m_ClickData.wstrObjKey, L"UI");
	lstrcpy(m_ClickData.wstrStateKey, L"TownButton");
	m_ClickData.byDrawID = 7;


	m_ImageData.vPos = D3DXVECTOR3(m_tUnit.vPos.x - 104.f, m_tUnit.vPos.y - 135.f, 0.f);
	m_ImageData.vSize = D3DXVECTOR3(80.f, 80.f, 0.f);
	lstrcpy(m_ImageData.wstrObjKey, L"Portrait");
	vector<OBJECT*>* pVector = CObjMgr::GetInstance()->FindGameObject<CPortrait>()->GetPortrait();
	for (size_t i = 0; i < pVector->size(); ++i)
	{
		if (lstrcmp((*pVector)[i]->wstrStateKey, L"PortraitUI") != 0)
		{
			if ((*pVector)[i]->byDrawID == 1)
			{
				lstrcpy(m_ImageData.wstrStateKey, (*pVector)[i]->wstrStateKey);
				break;
			}
		}
	}
	m_ImageData.byDrawID = 0;

	m_eRenderID = RENDER_TYPE_UI;
	return S_OK;
}
int	CInventory::Update(void)
{
	if (m_bClose)
		return 1;

	MakeRect();

	for (int i = 0; i < 8; ++i)
	{
		D3DXVECTOR3 vPos;
		if(i== ITEM_TYPE_WEAPON)
			vPos = m_tUnit.vPos - D3DXVECTOR3(17.f, 78.f, 0.f);
		else if(i==ITEM_TYPE_ARMOR)
			vPos = m_tUnit.vPos - D3DXVECTOR3(-50.f, 78.f, 0.f);
		else if (i == ITEM_TYPE_HEADER)
			vPos = m_tUnit.vPos - D3DXVECTOR3(-50.f, 142.f, 0.f);

		if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) != NULL)
		{
			CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i)->ItemPos(vPos);
		}
	}

	KeyInput();

	return 0;
}
void CInventory::Render(void)
{
	const TEX_INFO* pMainUITex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
											   m_tUnit.wstrStateKey,
											   m_tUnit.byDrawID);
	if (pMainUITex == NULL)
	{
		ERROR_MSG(L"Inven 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x 
		, m_tUnit.vPos.y 
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pMainUITex->ImageInfo.Width);
	float fY = CENTER_POS(pMainUITex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pMainUITex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	/////////////////////////////////////////////////////////////////
	const TEX_INFO* ButtonTex =
		CTextureMgr::GetInstance()->GetTexture(m_ClickData.wstrObjKey,
			m_ClickData.wstrStateKey,
			m_ClickData.byDrawID);
	if (ButtonTex == NULL)
	{
		ERROR_MSG(L"button 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_ClickData.vPos.x 
		, m_ClickData.vPos.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_ClickData.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_ClickData.matWorld);

	float fX_button = CENTER_POS(ButtonTex->ImageInfo.Width);
	float fY_button = CENTER_POS(ButtonTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		ButtonTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX_button, fY_button, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, m_ClickData.vPos.x - 30, m_ClickData.vPos.y - 5, 0.f);

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


	/////////////////////////////////////////////////////////////////
	const TEX_INFO* PortraiTex =
		CTextureMgr::GetInstance()->GetTexture(m_ImageData.wstrObjKey,
					    					   m_ImageData.wstrStateKey,
											   m_ImageData.byDrawID);
	if (PortraiTex == NULL)
	{
		ERROR_MSG(L"button 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_ImageData.vPos.x
		, m_ImageData.vPos.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_ImageData.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_ImageData.matWorld);

	float fX_button2 = CENTER_POS(PortraiTex->ImageInfo.Width);
	float fY_button2 = CENTER_POS(PortraiTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		PortraiTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX_button2, fY_button2, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	CItem* pItem[10];
	for (int i = 0; i < 10; ++i)
		pItem[i] = NULL;

	TCHAR selectWstrstatekey[MIDDLE_STR];

	lstrcpy(selectWstrstatekey, CObjMgr::GetInstance()->FindGameObject<CPortrait>()->selectUnit);

	for (int i = 0; i < 10; ++i)
	{
		if(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i) != NULL)
			CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i)->Render();
	}

	for (int i = 0; i < 8; ++i)
	{
		if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) != NULL)
			CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i)->Render();
	}

	//for (int i = 0; i < 10; ++i)
	//{
	//	Rectangle(m_hdc, m_InvenRect[i].left, m_InvenRect[i].top, m_InvenRect[i].right, m_InvenRect[i].bottom);
	//}

    //Rectangle(m_hdc, m_EquipInvenRect.left, m_EquipInvenRect.top, m_EquipInvenRect.right, m_EquipInvenRect.bottom);
}
void CInventory::Release(void)
{
	CObjMgr::GetInstance()->FindGameObject<CPortrait>()->SetOpenInven(false);
	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetOpenInven(false);
}

void CInventory::KeyInput()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	if (ObjectPicking(vMousePos))
		m_ClickData.byDrawID = 8;
	else
		m_ClickData.byDrawID = 7;

	if (InvenPicking(vMousePos) || m_bClickItem)
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetOpenInven(true);
	else
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetOpenInven(false);

	static bool SelectInven = FALSE;
	static bool SelectEquip = FALSE;

	static bool SelectItem = FALSE;

	int iSelectItem = 0;

	D3DXVECTOR3 vMouseItemPos = CMouse::GetMousePos();

	if (KEYPRESS(dwKey, LBUTTON))
	{
		for (int i = 0; i < 10; ++i)
		{
			if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i) != NULL)
			{
				if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i)->Picking(vMouseItemPos))
				{
					if (!SelectItem)
						m_iSelectIndex = i;

					SelectItem = TRUE;
					CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex)->GetItemInfo().vPos = vMouseItemPos;

					m_bClickItem = true;

					return;
				}
			}
		}
	}
	else
	{
		if (m_bClickItem)
		{
			for (int j = 0; j < 10; ++j)
			{
				RECT rc = {};
				if (IntersectRect(&rc, &(m_InvenRect[j]), &(CObjMgr::GetInstance()->FindGameObject<CMouse>()->GetLocateRect())))
				{
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j) != NULL)
					{
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SwapDragItem(m_iSelectIndex, j);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex)->SetPos(m_iSelectIndex);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j)->SetPos(j);
					}
					else
					{
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j) = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex) = NULL;
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j)->SetPos(j);
					}
					m_bClickItem = false;
					SelectItem = FALSE;
					return;
				}
				else if (IntersectRect(&rc, &(m_EquipInvenRect), &(CObjMgr::GetInstance()->FindGameObject<CMouse>()->GetLocateRect())))
				{
					CItem* pItem = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex);
					CObjMgr::GetInstance()->FindGameObject<CPlayer>()->EquipState(true);
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(pItem->GetItemInfo().byOption) == NULL)
					{
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(pItem->GetItemInfo().byOption) = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(pItem->GetItemInfo().byOption)->SetPos(pItem->GetItemInfo().byOption);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex) = NULL;

					}
					else
					{
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SwapItem(pItem->GetItemInfo().byOption, m_iSelectIndex);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex)->SetPos(m_iSelectIndex);
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(pItem->GetItemInfo().byOption)->SetPos(pItem->GetItemInfo().byOption);
					}
					m_bClickItem = false;
					SelectItem = FALSE;
					return;
				}
				else
				{
					if (j == 9 && m_bClickItem)
					{
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(m_iSelectIndex)->SetPos(m_iSelectIndex);
					}
				}
			}
		}
		m_bClickItem = false;
		SelectItem = FALSE;
	}

	if (KEYPRESS(dwKey, LBUTTON))
	{
		if (SelectInven == FALSE)
		{
			SelectInven = TRUE;
			if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
			{
				for (int i = 0; i < 8; ++i)
				{
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) != NULL)
					{
						if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i)->Picking(vMousePos))
						{
							for (int j = 0; j < 10; ++j)
							{
								if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j) == NULL)
								{
									CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j) = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i);
									CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(j)->SetPos(j);
									CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) = NULL;
									CObjMgr::GetInstance()->FindGameObject<CPlayer>()->EquipState(true);
									return;
								}
							}
						}
					}
				}
			}
			if (ObjectPicking(vMousePos))
			{
				m_bClose = true;
				return;
			}
		}
	}
	else
		SelectInven = FALSE;

	if (KEYPRESS(dwKey, RBUTTON))
	{
		if (SelectEquip == FALSE)
		{
			SelectEquip = TRUE;
			CItem* pItem[10];
			for (int i = 0; i < 10; ++i)
				pItem[i] = NULL;

			TCHAR selectWstrstatekey[MIDDLE_STR];

			lstrcpy(selectWstrstatekey, CObjMgr::GetInstance()->FindGameObject<CPortrait>()->selectUnit);

			if (CSceneMgr::GetInstance()->GetScene()==SCENE_CITY)
			{
				for (int i = 0; i < 10; ++i)
				{
					if (lstrcmp(selectWstrstatekey, L"Player") == 0)
						pItem[i] = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i);

					if (pItem[i] != NULL)
					{
						if ((pItem[i]->Picking(vMousePos)))
						{
							CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SellItem(i);
						}
					}
				}
			}
			else if(CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
			{
				for (int i = 0; i < 10; ++i)
				{
					if (lstrcmp(selectWstrstatekey, L"Player") == 0)
						pItem[i] = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i);

					if (pItem[i] != NULL)
					{
						if ((pItem[i]->Picking(vMousePos)))
						{
							CObjMgr::GetInstance()->FindGameObject<CPlayer>()->EquipState(true);
							if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(pItem[i]->GetItemInfo().byOption) == NULL)
							{
								CObjMgr::GetInstance()->FindGameObject<CPlayer>()->EquipItem(pItem[i]);
								CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SellItem(i);
							}
							else
							{
								CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SwapItem(pItem[i]->GetItemInfo().byOption, i);
								CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i)->SetPos(i);
							}
						}
					}
				}
			}
		}
	}
	else
		SelectEquip = FALSE;

}

bool CInventory::ObjectPicking(const D3DXVECTOR3& vPos)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_ClickData.vPos.x,
		m_ClickData.vPos.y - (m_ClickData.vSize.y / 2.f),
		0.f),

		D3DXVECTOR3(m_ClickData.vPos.x + (m_ClickData.vSize.x / 2.f),
		m_ClickData.vPos.y,
		0.f),

		D3DXVECTOR3(m_ClickData.vPos.x,
		m_ClickData.vPos.y + (m_ClickData.vPos.y / 2.f),
		0.f),

		D3DXVECTOR3(m_ClickData.vPos.x - (m_ClickData.vSize.x / 2.f),
		m_ClickData.vPos.y,
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

bool CInventory::InvenPicking(const D3DXVECTOR3& vPos)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_tUnit.vPos.x,
		m_tUnit.vPos.y - (m_tUnit.vSize.y),
		0.f),

		D3DXVECTOR3(m_tUnit.vPos.x + (m_tUnit.vSize.x),
		m_tUnit.vPos.y,
		0.f),

		D3DXVECTOR3(m_tUnit.vPos.x,
		m_tUnit.vPos.y + (m_tUnit.vPos.y),
		0.f),

		D3DXVECTOR3(m_tUnit.vPos.x - (m_tUnit.vSize.x),
		m_tUnit.vPos.y,
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


void CInventory::MakeRect()
{
	float fX = 128.f;
	float fY = -58.f;
	for (int i = 0; i<10; ++i)
	{

		D3DXVECTOR3 vPos = m_tUnit.vPos - D3DXVECTOR3(fX, fY, 0.f);

		m_InvenRect[i].left = int(vPos.x - 30.f);
		m_InvenRect[i].top = int(vPos.y - 30.f);
		m_InvenRect[i].right = int(vPos.x + 30.f);
		m_InvenRect[i].bottom = int(vPos.y + 30.f);
		fX -= 64.f;
		if (i == 4)
		{
			fX = 128.f;
			fY -= 64.f;
		}
	}

	D3DXVECTOR3 vPos1 = m_tUnit.vPos - D3DXVECTOR3(-45.f, 80.f, 0.f);

	m_EquipInvenRect.left = int(vPos1.x - 95.f);
	m_EquipInvenRect.top = int(vPos1.y - 98.f);
	m_EquipInvenRect.right = int(vPos1.x + 95.f);
	m_EquipInvenRect.bottom = int(vPos1.y + 98.f);
}