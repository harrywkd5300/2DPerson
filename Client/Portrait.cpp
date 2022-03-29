#include "stdafx.h"
#include "Portrait.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Inventory.h"
#include "Factory.h"
#include "SceneMgr.h"
#include "Player.h"
#include "SoundMgr.h"
#include "TimeMgr.h"

CPortrait::CPortrait()
	:m_ViewPortrait(false), m_bOpenInven(false), m_bSelectPortait(true), m_fAddTime(0.f)
	, m_fPlusTime(0.f)
{
}


CPortrait::~CPortrait()
{
	Release();
}

HRESULT	CPortrait::Initialize(void)
{
	SetPortrait();
	m_eRenderID = RENDER_TYPE_UI;
	return S_OK;
}

int		CPortrait::Update(void)
{
	if(!m_ViewPortrait)
		SelectObject();


	SelectPortait();


	return 0;
}

void	CPortrait::Render(void)
{
	if (!m_ViewPortrait)
	{
		for (size_t i = 0; i < vecPortrait.size(); ++i)
		{
			const TEX_INFO* pPortraitTex =
				CTextureMgr::GetInstance()->GetTexture(vecPortrait[i]->wstrObjKey,
					vecPortrait[i]->wstrStateKey,
					vecPortrait[i]->byDrawID);
			if (pPortraitTex == NULL)
			{
				ERROR_MSG(L"플레이어 vecPortrait 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, vecPortrait[i]->vPos.x
				, vecPortrait[i]->vPos.y
				, 0.f);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

			vecPortrait[i]->matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&vecPortrait[i]->matWorld);

			float fX = CENTER_POS(pPortraitTex->ImageInfo.Width);
			float fY = CENTER_POS(pPortraitTex->ImageInfo.Height);

			CDevice::GetInstance()->GetSprite()->Draw(
				pPortraitTex->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(vecPortrait[i]->byOption, 255, 255, 255));
		}
	}
}

void	CPortrait::Release(void)
{
	for (size_t i = 0; i < vecPortrait.size(); ++i)
	{
		safeDelete(vecPortrait[i]);
	}
	vecPortrait.clear();
}

void CPortrait::SetPortrait()
{
	//Portrait 그리기!!
	OBJECT* m_Portrait0 = new OBJECT;
	m_Portrait0->vPos = D3DXVECTOR3(55.f, 70.f, 0.f);
	m_Portrait0->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait0->vSize = D3DXVECTOR3(84.f, 32.f, 0.f);
	m_Portrait0->byDrawID = 1;
	m_Portrait0->byOption = 255;
	lstrcpy(m_Portrait0->wstrStateKey, L"Player");
	lstrcpy(m_Portrait0->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait0);

	OBJECT* m_Portrait1 = new OBJECT;
	m_Portrait1->vPos = D3DXVECTOR3(50.f, 110.f, 0.f);
	m_Portrait1->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait1->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait1->byDrawID = 1;
	m_Portrait1->byOption = 160;
	lstrcpy(m_Portrait1->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait1->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait1);

	OBJECT* m_Portrait2 = new OBJECT;
	m_Portrait2->vPos = D3DXVECTOR3(50.f, 150.f, 0.f);
	m_Portrait2->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait2->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait2->byDrawID = 1;
	m_Portrait2->byOption = 160;
	lstrcpy(m_Portrait2->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait2->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait2);

	OBJECT* m_Portrait3 = new OBJECT;
	m_Portrait3->vPos = D3DXVECTOR3(50.f, 190.f, 0.f);
	m_Portrait3->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait3->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait3->byDrawID = 1;
	m_Portrait3->byOption = 160;
	lstrcpy(m_Portrait3->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait3->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait3);

	OBJECT* m_Portrait4 = new OBJECT;
	m_Portrait4->vPos = D3DXVECTOR3(50.f, 230.f, 0.f);
	m_Portrait4->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait4->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait4->byDrawID = 1;
	m_Portrait4->byOption = 160;
	lstrcpy(m_Portrait4->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait4->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait4);

	OBJECT* m_Portrait5 = new OBJECT;
	m_Portrait5->vPos = D3DXVECTOR3(50.f, 270.f, 0.f);
	m_Portrait5->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait5->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait5->byDrawID = 1;
	m_Portrait5->byOption = 160;
	lstrcpy(m_Portrait5->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait5->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait5);

	OBJECT* m_Portrait6 = new OBJECT;
	m_Portrait6->vPos = D3DXVECTOR3(50.f, 310.f, 0.f);
	m_Portrait6->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait6->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait6->byDrawID = 1;
	m_Portrait6->byOption = 160;
	lstrcpy(m_Portrait6->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait6->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait6);

	OBJECT* m_Portrait7 = new OBJECT;
	m_Portrait7->vPos = D3DXVECTOR3(50.f, 350.f, 0.f);
	m_Portrait7->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait7->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait7->byDrawID = 1;
	m_Portrait7->byOption = 160;
	lstrcpy(m_Portrait7->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait7->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait7);

	OBJECT* m_Portrait8 = new OBJECT;
	m_Portrait8->vPos = D3DXVECTOR3(50.f, 390.f, 0.f);
	m_Portrait8->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait8->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait8->byDrawID = 1;
	m_Portrait8->byOption = 160;
	lstrcpy(m_Portrait8->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait8->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait8);

	OBJECT* m_Portrait9 = new OBJECT;
	m_Portrait9->vPos = D3DXVECTOR3(50.f, 430.f, 0.f);
	m_Portrait9->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait9->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait9->byDrawID = 1;
	m_Portrait9->byOption = 160;
	lstrcpy(m_Portrait9->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait9->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait9);

	OBJECT* m_Portrait10 = new OBJECT;
	m_Portrait10->vPos = D3DXVECTOR3(50.f, 470.f, 0.f);
	m_Portrait10->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Portrait10->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
	m_Portrait10->byDrawID = 1;
	m_Portrait10->byOption = 160;
	lstrcpy(m_Portrait10->wstrStateKey, L"PortraitUI");
	lstrcpy(m_Portrait10->wstrObjKey, L"Portrait");
	vecPortrait.push_back(m_Portrait10);
}

void CPortrait::SelectObject()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	//vMousePos -= m_vScroll;

	int iIndex = GetCityIndex(vMousePos);

	if (iIndex != -1)
	{
		DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

		static bool SelectPortrait = FALSE;

		if (KEYPRESS(dwKey, LBUTTON))
		{
			if (SelectPortrait == FALSE)
			{
				SelectPortrait = TRUE;

				if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"PortraitUI") != 0)
				{
					for (size_t i = 0; i < vecPortrait.size(); ++i)
					{
						if (lstrcmp(vecPortrait[i]->wstrStateKey, L"PortraitUI") != 0)
						{
							vecPortrait[i]->byDrawID = 2;
							vecPortrait[i]->vPos.x = 50.f;
							vecPortrait[i]->vSize = D3DXVECTOR3(72.f, 26.f, 0.f);
						}
						else
							break;
					}
					vecPortrait[iIndex]->byDrawID = 1;
					vecPortrait[iIndex]->vPos.x = 55.f;
					vecPortrait[iIndex]->vSize = D3DXVECTOR3(84.f, 32.f, 0.f);
					lstrcpy(selectUnit, vecPortrait[iIndex]->wstrStateKey);

					if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Player") == 0)
					{
						//CSoundMgr::GetInstance()->PlayBGM(L"Player_select.wav", CSoundMgr::Channel_Player);
						//플레이어6초, 이순신7초, 
						CSoundMgr::GetInstance()->PlaySound(L"Player_select.wav", CSoundMgr::Channel_Unit0);
						m_fPlusTime = 6.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"LeeSoonsin") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Leesonnsin_select.wav", CSoundMgr::Channel_Unit1);
						m_fPlusTime = 7.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Jeungjang") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Juang_select.wav", CSoundMgr::Channel_Unit2);
						m_fPlusTime = 7.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Guangmok") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Guangmok_select.wav", CSoundMgr::Channel_Unit3);
						m_fPlusTime = 9.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"FireBird") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"FireBird_select.wav", CSoundMgr::Channel_Unit4);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Dragon") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Dragon_select.wav", CSoundMgr::Channel_Unit5);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Damoon") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Damoon_select.wav", CSoundMgr::Channel_Unit6);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"SoonbiYean") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Soon_select.wav", CSoundMgr::Channel_Unit7);
						m_fPlusTime = 6.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Tiger") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Tiger_select.wav", CSoundMgr::Channel_Unit8);
						m_fPlusTime = 9.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Turtle") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Turtle_select.wav", CSoundMgr::Channel_Unit9);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
					else if (lstrcmp(vecPortrait[iIndex]->wstrStateKey, L"Jiguk") == 0)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Jiguk_select.wav", CSoundMgr::Channel_Unit10);
						m_fPlusTime = 8.f;
						m_bSelectPortait = false;
					}
				}
			}
		}
		else if (KEYPRESS(dwKey, I))
		{
			if (SelectPortrait == FALSE)
			{
				SelectPortrait = TRUE;
				if (!m_bOpenInven)
				{
					CObjMgr::GetInstance()->AddGameObject(
						CFactory<CInventory>::CreateGameObject(),
						OBJ_TYPE_INVEN);
					m_bOpenInven = true;
				}
			}
		}
		else
		{
			SelectPortrait = FALSE;
		}
	}

}

bool CPortrait::Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject)
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

int CPortrait::GetCityIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < vecPortrait.size(); ++i)
	{
		if (Picking(vPos, vecPortrait[i]))
		{
			return i;
		}
	}
	return -1;
}

void CPortrait::SelectPortait()
{
	if (!m_bSelectPortait)
	{
		m_fAddTime += GET_TIME * 3.f;
		if (m_fAddTime > m_fPlusTime)
		{
			m_fAddTime = 0.f;
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Player);
			m_bSelectPortait = true;
		}
	}
}