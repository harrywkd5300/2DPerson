
#include "StdAfx.h"
#include "Mouse.h"
#include "Device.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "SceneMgr.h"
#include "Player.h"

HRESULT	CMouse::Initialize(void)
{
	wstrObjKey = L"Mouse";
	m_bFirstClick = true;
	wstrStateKey = L"Stand_Idle";
	m_bClick2 = false;
	m_tFrame = FRAME(0.f, 8.f, 15.f);


	m_hdc = GetDC(g_hwnd);
	m_bMakeRect = false;
	m_bClick = false;
	m_bClickTime = 0.f;
	m_eRenderID = RENDER_TYPE_MOUSE;

	m_clickPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_clickAgoPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_clickUIAgoPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	return S_OK;
}
int	CMouse::Update(void)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hwnd, &ptMouse);

	D3DXVECTOR3 vScroll;

	if (CSceneMgr::GetInstance()->GetScene() == SCENE_CITY)
	{
		vScroll.x = 0.f;
		vScroll.y = 0.f;
	}
	else
	{
		vScroll.x = m_vScroll.x;
		vScroll.y = m_vScroll.y;
	}

	m_tUnit.vPos.x = float(ptMouse.x);
	m_tUnit.vPos.y = float(ptMouse.y);
	m_tUnit.vPos.z = 0.f;

	m_clickUIAgoPos.x = float(ptMouse.x);
	m_clickUIAgoPos.y = float(ptMouse.y);
	m_clickUIAgoPos.z = 0.f;

	m_LocateRect.left = int(m_tUnit.vPos.x + vScroll.x - 5.f);
	m_LocateRect.top = int(m_tUnit.vPos.y + vScroll.y - 5.f);
	m_LocateRect.right = int(m_tUnit.vPos.x + vScroll.x + 5.f);
	m_LocateRect.bottom = int(m_tUnit.vPos.y + vScroll.y + 5.f);

	if (!CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetOpenInven())
	{
		list<CGameObject*>::iterator iter = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).begin();
		list<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).end();
		for (; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vMousePos = D3DXVECTOR3(m_tUnit.vPos.x, m_tUnit.vPos.y, 0.f);

			vMousePos -= m_vScroll;

			if (dynamic_cast<CMonster*>((*iter))->Picking(vMousePos))
			{
				if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
				{
					wstrStateKey = L"Attack_Idle";
					m_tFrame.fMax = 17.f;
					break;
				}
			}
			else
			{
				wstrStateKey = L"Stand_Idle";
				m_tFrame.fMax = 15.f;
			}
		}
	}

	if (m_bClick2)
	{
		wstrStateKey = L"Stand_Click";
		m_tFrame.fMax = 5.f;
	}

	FrameCheck();

	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	static bool SelectMouse = FALSE;

	if (KEYPRESS(dwKey, RBUTTON))
	{
		if (SelectMouse == FALSE)
		{
			SelectMouse = TRUE;
			if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
			{
				m_clickPos.x = float(ptMouse.x) + m_vScroll.x;
				m_clickPos.y = float(ptMouse.y) + m_vScroll.y;
			}
		}
	}
	else
	{
		if (SelectMouse == TRUE)
		{
			if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
			{
				m_bFirstClick = true;
				m_bMakeRect = true;
				m_bClick = true;
				m_clickAgoPos.x = float(ptMouse.x) + m_vScroll.x;
				m_clickAgoPos.y = float(ptMouse.y) + m_vScroll.y;
			}
			SelectMouse = FALSE;
		}
	}

	if (m_bMakeRect)
	{
		m_clickRect.left   = int(((m_clickAgoPos.x + m_clickPos.x) / 2.f) - ((m_clickAgoPos.x - m_clickPos.x) / 2.f));
		m_clickRect.top    = int(((m_clickAgoPos.y + m_clickPos.y) / 2.f) - ((m_clickAgoPos.y - m_clickPos.y) / 2.f));
		m_clickRect.right  = int(((m_clickAgoPos.x + m_clickPos.x) / 2.f) + ((m_clickAgoPos.x - m_clickPos.x) / 2.f));
		m_clickRect.bottom = int(((m_clickAgoPos.y + m_clickPos.y) / 2.f) + ((m_clickAgoPos.y - m_clickPos.y) / 2.f));
	}
	else
	{
		m_clickRect.left = 0;
		m_clickRect.top = 0;
		m_clickRect.right = 0;
		m_clickRect.bottom = 0;
	}

	if (m_bMakeRect)
	{
		m_bClickTime += GET_TIME * 5.f;
		if (m_bClickTime > 3.f)
		{
			m_bClickTime = 0.f;
			m_bMakeRect = false;
			m_bClick = false;
		}
	}
	return 0;
}
void CMouse::Render(void)
{
	const TEX_INFO* pPortraitTex =
		CTextureMgr::GetInstance()->GetTexture(wstrObjKey.c_str(),
											   wstrStateKey.c_str(),
											   static_cast<int>(m_tFrame.fIndex));
	if (pPortraitTex == NULL)
	{
		ERROR_MSG(L"마우스 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x
		, m_tUnit.vPos.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pPortraitTex->ImageInfo.Width)  - 5.f;
	float fY = CENTER_POS(pPortraitTex->ImageInfo.Height) -10.f;

	CDevice::GetInstance()->GetSprite()->Draw(
		pPortraitTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	ShowCursor(false);

	if (m_bMakeRect)
	{
		const TEX_INFO* pRectTex =
			CTextureMgr::GetInstance()->GetTexture(L"UI",
				L"Rect",
				0);
		if (pRectTex == NULL)
		{
			ERROR_MSG(L"RECT 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, (m_clickAgoPos.x + m_clickPos.x) / 2.f + m_vScroll.x
			, (m_clickAgoPos.y + m_clickPos.y) / 2.f + m_vScroll.y
			, 0.f);

		D3DXMatrixScaling(&matScale, (m_clickAgoPos.x - m_clickPos.x), (m_clickAgoPos.y - m_clickPos.y), 0.f);

		m_tUnit.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

		float fX = CENTER_POS(pRectTex->ImageInfo.Width);
		float fY = CENTER_POS(pRectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->Draw(
			pRectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(150, 255, 255, 255));
	}
}
void CMouse::Release(void)
{
	DeleteDC(m_hdc);
}

void CMouse::FrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_tFrame.fIndex > m_tFrame.fMax)
	{
		m_bClick2 = false;
		m_tFrame.fIndex = 0.f;
	}
}