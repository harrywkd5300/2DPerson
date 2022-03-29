#include "StdAfx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Mouse.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "Factory.h"
#include "DataSubject.h"
#include "Tower.h"
#include "Portrait.h"
#include "Monster.h"
#include "SceneMgr.h"
#include "Inventory.h"
#include "BattleCount.h"
#include "Count.h"
#include "MouseClick.h"

CPlayer::CPlayer(void)
	:m_fScaleX(1.f), m_iWayCount(2), m_bNotMove(false), m_bViewPlayer(true),
	iTileIndex(0), m_fWalkLength(0.f), m_bOpenInven(false), m_iEquipItem(false),
	m_iFirstRender(0), m_iChangeC(0), m_SceneChange(false), m_fAddTime(0.f), m_fPlusTime(0.f)
	, fDist(0.f), m_bStopMove(false)
{
	for (int i = 0; i < 10; ++i)
		m_Inven[i] = NULL;

	for (int i = 0; i < 10; ++i)
		m_EquipInven[i] = NULL;
}

CPlayer::~CPlayer(void)
{
	Release();
}

void CPlayer::SetPlayerPos(float fX, float fY)
{
	for (size_t i = 0; i < vecPlayer.size(); ++i)
	{
		vecPlayer[i]->vPos.x = fX;
		vecPlayer[i]->vPos.y = fY;
	}
}
HRESULT	CPlayer::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(100.f, 300.f, 0.f);
	DrawPlayer();
	AStar = new CAstar;
	m_eCurState = Stand_LD;
	m_fSpeed = 200.f;
	m_eRenderID = RENDER_TYPE_OBJECT;
	m_iHp = 100.f;

	lstrcpy(pWeapon.wstrStateKey, L"Stand1");
	vecEmploy.push_back(this);

	m_tUnit.vPos = m_tInfo.vPos;
	m_tUnit.vSize = D3DXVECTOR3(98.f, 40.f, 0.f);

	//m_hdc = GetDC(g_hwnd);

	m_tFrame = FRAME(0.f, 8.f, 8.f);
	m_tWeapon = FRAME(0.f, 8.f, 8.f);

	m_hdc = GetDC(g_hwnd);

	return S_OK;
}
int CPlayer::Update(void)
{
	m_tUnit.vPos.x = m_tInfo.vPos.x;
	m_tUnit.vPos.y = m_tInfo.vPos.y;

	m_tUnit.vSize = D3DXVECTOR3(98.f, 74.f, 0.f);

	m_CollRect.left = int(m_tInfo.vPos.x + m_vScroll.x - 20.f);
	m_CollRect.top = int(m_tInfo.vPos.y + m_vScroll.y - 20.f);
	m_CollRect.right = int(m_tInfo.vPos.x + m_vScroll.x + 20.f);
	m_CollRect.bottom = int(m_tInfo.vPos.y + m_vScroll.y + 20.f);

	for (size_t i = 0; i < vecPlayer.size(); ++i)
	{
		vecPlayer[i]->vPos = m_tInfo.vPos;
		vecPlayer[i]->vDir = m_tInfo.vDir;
	}
	if (!m_fAttack)
	{
		m_fAddTime += GET_TIME * 3.f;
		if (m_fAddTime > 9.f)
		{
			m_fAddTime = 0.f;
			m_fAttack = true;
		}
	}
	if (m_bViewPlayer)
	{
		if (m_EquipInven[ITEM_TYPE_WEAPON] != NULL)
			pWeapon.vPos = m_tInfo.vPos;
		if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
		{
			if (!m_bNotMove)
			{
				if (!m_bOpenInven)
					KeyInput();
				AStarMove();
				MyFrameCheck();
				StateChange();
			}
		}
		else if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
		{
			m_clickRect.left = int(m_tInfo.vPos.x + m_vScroll.x - 20);
			m_clickRect.top = int(m_tInfo.vPos.y + m_vScroll.y - 20);
			m_clickRect.right = int(m_tInfo.vPos.x + m_vScroll.x + 20);
			m_clickRect.bottom = int(m_tInfo.vPos.y + m_vScroll.y + 20);

			if (m_bClickUnit)
				DGKeyInput();
			StateChange();
			MyFrameCheck();
		}
	}
	return 0;
}
void CPlayer::Render(void)
{
	if (m_bViewPlayer)
	{
		for (size_t i = 0; i < vecPlayer.size(); ++i)
		{
			const TEX_INFO* pPlayerTex =
				CTextureMgr::GetInstance()->GetTexture((vecPlayer[i]->wstrObjectKey.c_str()),
				(vecPlayer[i]->wstrStateKey.c_str()),
					static_cast<int>(m_tFrame.fIndex));
			if (pPlayerTex == NULL)
			{
				ERROR_MSG(L"플레이어 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, vecPlayer[i]->vPos.x + m_vScroll.x
				, vecPlayer[i]->vPos.y + m_vScroll.y
				, 0.f);

			D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 0.f);

			vecPlayer[i]->matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&vecPlayer[i]->matWorld);

			float fX = CENTER_POS(pPlayerTex->ImageInfo.Width);
			float fY = CENTER_POS(pPlayerTex->ImageInfo.Height) + 25.f;

			CDevice::GetInstance()->GetSprite()->Draw(
				pPlayerTex->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (m_EquipInven[ITEM_TYPE_WEAPON] != NULL)
		{
			if (m_EquipInven[ITEM_TYPE_WEAPON] != NULL)
			{
				if (lstrcmp(m_EquipInven[ITEM_TYPE_WEAPON]->GetItemInfo().wstrStateKey, L"Gumangdo") == 0)
					lstrcpy(pWeapon.wstrObjKey, L"Weapon_Gumangdo");
				else if (lstrcmp(m_EquipInven[ITEM_TYPE_WEAPON]->GetItemInfo().wstrStateKey, L"ThunderGun") == 0)
					lstrcpy(pWeapon.wstrObjKey, L"Weapon_ThunderGun");
			}

			const TEX_INFO* pWeaponTex =
				CTextureMgr::GetInstance()->GetTexture(pWeapon.wstrObjKey,
					pWeapon.wstrStateKey,
					static_cast<int>(m_tFrame.fIndex));
			if (pWeaponTex == NULL)
			{
				ERROR_MSG(L"무기 이미지 X");
				return;
			}

			D3DXMatrixTranslation(&matTrans
				, pWeapon.vPos.x + m_vScroll.x
				, pWeapon.vPos.y + m_vScroll.y
				, 0.f);

			D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 0.f);

			pWeapon.matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&pWeapon.matWorld);

			float fX = CENTER_POS(pWeaponTex->ImageInfo.Width);
			float fY = CENTER_POS(pWeaponTex->ImageInfo.Height) + 25.f;

			CDevice::GetInstance()->GetSprite()->Draw(
				pWeaponTex->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CPlayer::Release(void)
{
	safeDelete(AStar);

	for (size_t i = 0; i < vecPlayer.size(); ++i)
	{
		safeDelete(vecPlayer[i]);
	}
	vecPlayer.clear();

	for (size_t i = 0; i < vecEmploy.size(); ++i)
	{
		safeDelete(vecEmploy[i]);
	}
	vecEmploy.clear();
}

void CPlayer::KeyInput()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos -= m_vScroll;

	static bool SelectPlayer = FALSE;

	if (KEYPRESS(dwKey, LBUTTON))
	{
		if (SelectPlayer == FALSE)
		{
			SelectPlayer = TRUE;
			if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
			{
				CObjMgr::GetInstance()->AddGameObject(
					CFactory<CMouseClick>::CreateGameObject(vMousePos.x, vMousePos.y, 0.f),
					OBJ_TYPE_UI);

				CObjMgr::GetInstance()->FindGameObject<CMouse>()->SetClick2(true);
				list<CGameObject*>::iterator iter = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).begin();
				list<CGameObject*>::iterator iter_end = CObjMgr::GetInstance()->GetList(OBJ_TYPE_MONSTER).end();
				for (; iter != iter_end; ++iter)
				{
					if (dynamic_cast<CMonster*>((*iter))->Picking(vMousePos))
					{
						D3DXVECTOR3 fDist = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - (*iter)->GetUnitInfo()->vPos;
						float fPlayerCost = D3DXVec3Length(&fDist);
						if (fPlayerCost < 80.f)
						{
							m_iStartPos = vMousePos;

							SetMonster((*iter)->GetUnitInfo());
							CObjMgr::GetInstance()->FindGameObject<CPortrait>()->SetPortrait(true);

							m_bNotMove = true;

							CObjMgr::GetInstance()->AddGameObject(
								CFactory<CBattleCount>::CreateGameObject(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f),
								OBJ_TYPE_UI);
							
							CObjMgr::GetInstance()->AddGameObject(
								CFactory<CBattleCount>::CreateGameObject((*iter)->GetUnitInfo()->vPos.x, (*iter)->GetUnitInfo()->vPos.y, 0.f),
								OBJ_TYPE_UI);
							(*iter)->SetNotMove(true);

							CObjMgr::GetInstance()->AddGameObject(
								CFactory<CCount>::CreateGameObject(m_tInfo.vPos.x, m_tInfo.vPos.y - 100.f, 0.f),
								OBJ_TYPE_UI);
							return;
						}
					}
				}
				list<CGameObject*>::iterator iter_begin = CObjMgr::GetInstance()->GetList(OBJ_TYPE_OBJECT).begin();
				list<CGameObject*>::iterator iter_end2 = CObjMgr::GetInstance()->GetList(OBJ_TYPE_OBJECT).end();
				for (; iter_begin != iter_end2; ++iter_begin)
				{
					if (dynamic_cast<CTower*>((*iter_begin))->Picking(vMousePos))
						dynamic_cast<CTower*>((*iter_begin))->bObjectClick = true;
				}
			}


			list<CGameObject*>::iterator iter_portait = CObjMgr::GetInstance()->GetList(OBJ_TYPE_PORTRAIT).begin();

			vector<OBJECT*>::iterator iter_pt = dynamic_cast<CPortrait*>((*iter_portait))->GetPortrait()->begin();
			vector<OBJECT*>::iterator iter_pt_end = dynamic_cast<CPortrait*>((*iter_portait))->GetPortrait()->end();

			D3DXVECTOR3 vMousePortraitPos = CMouse::GetMousePos();

			for (; iter_pt != iter_pt_end; ++iter_pt)
			{
				if (dynamic_cast<CPortrait*>(*iter_portait)->Picking(vMousePortraitPos, (*iter_pt)))
				{
					return;
				}
				else
				{
					if(iter_pt == iter_pt_end - 1)
					{
						m_EndPos = vMousePos;
						AStar->AStarStartPos(m_tInfo.vPos, vMousePos);
					}
				}
			}
		}
	}
	else if (KEYPRESS(dwKey, P))
	{
		if (m_bClick)
		{
			m_bClick = false;
			if (CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTilemap())
				CObjMgr::GetInstance()->FindGameObject<CBackGround>()->SetTilemap(false);
			else
				CObjMgr::GetInstance()->FindGameObject<CBackGround>()->SetTilemap(true);
		}
	}
	else
	{
		SelectPlayer = FALSE;
	}


	if (!m_bClick)
	{
		m_fClickTime += GET_TIME;
		if (m_fClickTime > 0.5f)
		{
			m_fClickTime = 0.f;
			m_bClick = true;
		}
	}

}
void CPlayer::StateChange()
{
	if (m_eCurState != m_ePreState || m_iEquipItem)
	{
		if (m_EquipInven[ITEM_TYPE_ARMOR] != NULL)
		{
			if (lstrcmp(m_EquipInven[ITEM_TYPE_ARMOR]->GetItemInfo().wstrStateKey, L"Beauti_Body") == 0)
			{
				vecPlayer[1]->wstrObjectKey = L"Defend_BeautiRight";
				vecPlayer[3]->wstrObjectKey = L"Defend_BeautiBody";
				vecPlayer[5]->wstrObjectKey = L"Defend_BeautiLeft";
			}
			else if (lstrcmp(m_EquipInven[ITEM_TYPE_ARMOR]->GetItemInfo().wstrStateKey, L"Dragon_Body") == 0)
			{
				vecPlayer[1]->wstrObjectKey = L"Defend_DragonRight";
				vecPlayer[3]->wstrObjectKey = L"Defend_DragonBody";
				vecPlayer[5]->wstrObjectKey = L"Defend_DragonLeft";
			}
		}
		else
		{
			vecPlayer[1]->wstrObjectKey = L"Player_rightArmy";
			vecPlayer[3]->wstrObjectKey = L"Player_body";
			vecPlayer[5]->wstrObjectKey = L"Player_leftArmy";
		}
		if (m_EquipInven[ITEM_TYPE_HEADER] != NULL)
		{
			if (lstrcmp(m_EquipInven[ITEM_TYPE_HEADER]->GetItemInfo().wstrStateKey, L"Beauti_Head") == 0)
				vecPlayer[4]->wstrObjectKey = L"Defend_BeautiHead";
			else if (lstrcmp(m_EquipInven[ITEM_TYPE_HEADER]->GetItemInfo().wstrStateKey, L"Dragon_Head") == 0)
				vecPlayer[4]->wstrObjectKey = L"Defend_DragonHead";
		}
		else
			vecPlayer[4]->wstrObjectKey = L"Player_Head";

		if (m_EquipInven[ITEM_TYPE_WEAPON] != NULL)
		{
			pWeapon.vPos = m_tInfo.vPos;
			if (lstrcmp(m_EquipInven[ITEM_TYPE_WEAPON]->GetItemInfo().wstrStateKey, L"Gumangdo") == 0)
				lstrcpy(pWeapon.wstrObjKey, L"Weapon_Gumangdo");
			else if (lstrcmp(m_EquipInven[ITEM_TYPE_WEAPON]->GetItemInfo().wstrStateKey, L"ThunderGun") == 0)
				lstrcpy(pWeapon.wstrObjKey, L"Weapon_ThunderGun");
		}

		switch (m_eCurState)
		{
		////////////Stand////////////////////////
		case Stand_down:    // down
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand5";
			lstrcpy(pWeapon.wstrStateKey, L"Stand5");
			break;
		case Stand_up:    // up
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand4";
			lstrcpy(pWeapon.wstrStateKey, L"Stand4");
			break;
		case Stand_right:    // right
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand2";
			lstrcpy(pWeapon.wstrStateKey, L"Stand2");
			break;
		case Stand_left: // left
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand2";
			lstrcpy(pWeapon.wstrStateKey, L"Stand2");
			break;
		case Stand_LU: // leftup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand3";
			lstrcpy(pWeapon.wstrStateKey, L"Stand3");
			break;
		case Stand_RU: // rightup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand3";
			lstrcpy(pWeapon.wstrStateKey, L"Stand3");
			break;
		case Stand_LD: // leftdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand1";
			lstrcpy(pWeapon.wstrStateKey, L"Stand1");
			break;
		case Stand_RD: // rightdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Stand1";
			lstrcpy(pWeapon.wstrStateKey, L"Stand1");
			break;

		////////////Walk////////////////////////
		case Walk_down:    // down
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move5";
			lstrcpy(pWeapon.wstrStateKey, L"Move5");
			break;
		case Walk_up:    // up
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move4";
			lstrcpy(pWeapon.wstrStateKey, L"Move4");
			break;
		case Walk_right:    // right
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move2";
			lstrcpy(pWeapon.wstrStateKey, L"Move2");
			break;
		case Walk_left: // left
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_iFirstRender = 1;
			m_fScaleX = 1.f;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move2";
			lstrcpy(pWeapon.wstrStateKey, L"Move2");
			break;
		case Walk_LU: // leftup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move3";
			lstrcpy(pWeapon.wstrStateKey, L"Move3");
			break;
		case Walk_RU: // rightup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move3";
			lstrcpy(pWeapon.wstrStateKey, L"Move3");
			break;
		case Walk_LD: // leftdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = 1.f;
			m_iFirstRender = 1;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move1";
			lstrcpy(pWeapon.wstrStateKey, L"Move1");
			break;
		case Walk_RD: // rightdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			m_tWeapon.fIndex = 0.f;
			m_tWeapon.fMax = 8.f;
			m_fScaleX = -1.f;
			m_iFirstRender = 0;
			for (size_t i = 0; i < vecPlayer.size(); ++i)
				vecPlayer[i]->wstrStateKey = L"Move1";
			lstrcpy(pWeapon.wstrStateKey, L"Move1");
			break;

			////////////Attack////////////////////////
		case Attack_down:    // down
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo")==0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword5";
				vecPlayer[2]->wstrStateKey = L"Attack_sword5";
				vecPlayer[3]->wstrStateKey = L"Attack_sword5";
				vecPlayer[4]->wstrStateKey = L"Attack_sword5";
				vecPlayer[5]->wstrStateKey = L"Attack_sword5";

				vecPlayer[0]->wstrStateKey = L"Attack5";
				vecPlayer[6]->wstrStateKey = L"Attack5";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword5");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun5";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun5";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun5";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun5";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun5";

				vecPlayer[0]->wstrStateKey = L"Attack5";
				vecPlayer[6]->wstrStateKey = L"Attack5";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun5");
			}
			break;
		case Attack_up:    // up
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword4";
				vecPlayer[2]->wstrStateKey = L"Attack_sword4";
				vecPlayer[3]->wstrStateKey = L"Attack_sword4";
				vecPlayer[4]->wstrStateKey = L"Attack_sword4";
				vecPlayer[5]->wstrStateKey = L"Attack_sword4";

				vecPlayer[0]->wstrStateKey = L"Attack4";
				vecPlayer[6]->wstrStateKey = L"Attack4";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword4");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun4";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun4";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun4";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun4";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun4";

				vecPlayer[0]->wstrStateKey = L"Attack4";
				vecPlayer[6]->wstrStateKey = L"Attack4";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun4");
			}
			break;
		case Attack_right:    // right
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_sword2";
				vecPlayer[2]->wstrStateKey = L"Attack_sword2";
				vecPlayer[3]->wstrStateKey = L"Attack_sword2";
				vecPlayer[4]->wstrStateKey = L"Attack_sword2";
				vecPlayer[5]->wstrStateKey = L"Attack_sword2";

				vecPlayer[0]->wstrStateKey = L"Attack2";
				vecPlayer[6]->wstrStateKey = L"Attack2";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword2");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun2";

				vecPlayer[0]->wstrStateKey = L"Attack2";
				vecPlayer[6]->wstrStateKey = L"Attack2";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun2");
			}
			break;
		case Attack_left: // left
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword2";
				vecPlayer[2]->wstrStateKey = L"Attack_sword2";
				vecPlayer[3]->wstrStateKey = L"Attack_sword2";
				vecPlayer[4]->wstrStateKey = L"Attack_sword2";
				vecPlayer[5]->wstrStateKey = L"Attack_sword2";

				vecPlayer[0]->wstrStateKey = L"Attack2";
				vecPlayer[6]->wstrStateKey = L"Attack2";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword2");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun2";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun2";

				vecPlayer[0]->wstrStateKey = L"Attack2";
				vecPlayer[6]->wstrStateKey = L"Attack2";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun2");
			}
			break;
		case Attack_LU: // leftup
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword3";
				vecPlayer[2]->wstrStateKey = L"Attack_sword3";
				vecPlayer[3]->wstrStateKey = L"Attack_sword3";
				vecPlayer[4]->wstrStateKey = L"Attack_sword3";
				vecPlayer[5]->wstrStateKey = L"Attack_sword3";

				vecPlayer[0]->wstrStateKey = L"Attack3";
				vecPlayer[6]->wstrStateKey = L"Attack3";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword3");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun3";

				vecPlayer[0]->wstrStateKey = L"Attack3";
				vecPlayer[6]->wstrStateKey = L"Attack3";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun3");
			}
			break;
		case Attack_RU: // rightup
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword3";
				vecPlayer[2]->wstrStateKey = L"Attack_sword3";
				vecPlayer[3]->wstrStateKey = L"Attack_sword3";
				vecPlayer[4]->wstrStateKey = L"Attack_sword3";
				vecPlayer[5]->wstrStateKey = L"Attack_sword3";

				vecPlayer[0]->wstrStateKey = L"Attack3";
				vecPlayer[6]->wstrStateKey = L"Attack3";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword3");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun3";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun3";

				vecPlayer[0]->wstrStateKey = L"Attack3";
				vecPlayer[6]->wstrStateKey = L"Attack3";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun3");
			}
			break;
		case Attack_LD: // leftdown
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword1";
				vecPlayer[2]->wstrStateKey = L"Attack_sword1";
				vecPlayer[3]->wstrStateKey = L"Attack_sword1";
				vecPlayer[4]->wstrStateKey = L"Attack_sword1";
				vecPlayer[5]->wstrStateKey = L"Attack_sword1";

				vecPlayer[0]->wstrStateKey = L"Attack1";
				vecPlayer[6]->wstrStateKey = L"Attack1";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword1");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = 1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun1";

				vecPlayer[0]->wstrStateKey = L"Attack1";
				vecPlayer[6]->wstrStateKey = L"Attack1";

				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun1");
			}
			break;
		case Attack_RD: // rightdown
			if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_Gumangdo") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 10.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 10.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 1;
				vecPlayer[1]->wstrStateKey = L"Attack_sword1";
				vecPlayer[2]->wstrStateKey = L"Attack_sword1";
				vecPlayer[3]->wstrStateKey = L"Attack_sword1";
				vecPlayer[4]->wstrStateKey = L"Attack_sword1";
				vecPlayer[5]->wstrStateKey = L"Attack_sword1";

				vecPlayer[0]->wstrStateKey = L"Attack1";
				vecPlayer[6]->wstrStateKey = L"Attack1";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_sword1");
			}
			else if (lstrcmp(pWeapon.wstrObjKey, L"Weapon_ThunderGun") == 0)
			{
				m_tFrame.fIndex = 0;
				m_tFrame.fMax = 8.f;
				m_tWeapon.fIndex = 0.f;
				m_tWeapon.fMax = 8.f;
				m_fScaleX = -1.f;
				m_iFirstRender = 0;
				vecPlayer[1]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[2]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[3]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[4]->wstrStateKey = L"Attack_Gun1";
				vecPlayer[5]->wstrStateKey = L"Attack_Gun1";

				vecPlayer[0]->wstrStateKey = L"Attack1";
				vecPlayer[6]->wstrStateKey = L"Attack1";
				lstrcpy(pWeapon.wstrStateKey, L"Attack_Gun1");
			}
			break;
		}
		m_iEquipItem = false;
	}
	m_ePreState = m_eCurState;
}

void CPlayer::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_tFrame.fIndex >= m_tFrame.fMax)
	{
		m_bBattle = false;
		m_bAttackTime = false;
		m_tFrame.fIndex = 0.f;
	}
}

void CPlayer::AStarMove(void)
{
	list<int>* pBestList = AStar->GetBestList();
	list<int>* pBestListNode = AStar->GetBestListNode();

	const vector<TILE*>* pVecTile
		= CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if (pVecTile == NULL)
		return;

	if (m_SceneChange || pBestList->empty())
	{
		m_SceneChange = false;

		if (m_iWay == WAY_UP)
			m_eCurState = Stand_up;
		else if(m_iWay == WAY_DOWN)
			m_eCurState = Stand_down;
		else if (m_iWay == WAY_RIGTH)
			m_eCurState = Stand_right;
		else if (m_iWay == WAY_LEFT)
			m_eCurState = Stand_left;
		else if (m_iWay == WAY_RIGTHUP)
			m_eCurState = Stand_RU;
		else if (m_iWay == WAY_RIGTHDOWN)
			m_eCurState = Stand_RD;
		else if (m_iWay == WAY_LEFTUP)
			m_eCurState = Stand_LU;
		else if (m_iWay == WAY_LEFTDOWN)
			m_eCurState = Stand_LD;
		return;
	}

	if (pBestListNode->empty())
		return;

	iTileIndex = AStar->GetTileIndex(m_tInfo.vPos);

	int iMoveIndex = pBestList->front();

	if (iTileIndex < 0)
		return;

	if ((*pVecTile)[iTileIndex]->byOption == 0)
		m_iChangeC = 0;
	else if ((*pVecTile)[iTileIndex]->byOption == 3)
		m_iChangeC = 3;

	(*pVecTile)[iTileIndex]->byOption = 4;
	
	if(m_iChangeC || (*pVecTile)[iTileIndex]->byOption == 3)
		m_fSpeed = 50.f;
	else
		m_fSpeed = 100.f;

	m_tInfo.vDir = (*pVecTile)[iMoveIndex]->vPos - m_tInfo.vPos;

	float fDist = D3DXVec3Length(&m_tInfo.vDir);

	if (iTileIndex != iMoveIndex)
	{
		m_fWalkLength += fDist;
		if (iTileIndex > 0)
			(*pVecTile)[iTileIndex]->byOption = m_iChangeC;
		iTileIndex = iMoveIndex;
	}

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * GET_TIME * m_fSpeed;

	for (size_t i = 0; i < vecPlayer.size(); ++i)
	{
		vecPlayer[i]->vPos = m_tInfo.vPos;
		vecPlayer[i]->vDir = m_tInfo.vDir;
	}

	if (fDist < 10.f)
	{
		pBestList->pop_front();

		if (!pBestList->empty())
			iMoveIndex = pBestList->front();

		if ((*pVecTile)[iMoveIndex]->byOption == 4)
		{
			AStar->AStarStartPos(m_tInfo.vPos, m_EndPos);
			return;
		}

		pBestListNode->pop_front();
	}

	if (pBestListNode->empty())
		return;

	if (pBestListNode->front() == WAY_RIGTH)
	{
		m_eCurState = Walk_right;
		m_iWay = WAY_RIGTH;
	}
	else if (pBestListNode->front() == WAY_LEFT)
	{
		m_eCurState = Walk_left;
		m_iWay = WAY_LEFT;
	}
	else if (pBestListNode->front() == WAY_LEFTDOWN)
	{
		m_eCurState = Walk_LD;
		m_iWay = WAY_LEFTDOWN;
	}
	else if (pBestListNode->front() == WAY_RIGTHDOWN)
	{
		m_eCurState = Walk_RD;
		m_iWay = WAY_RIGTHDOWN;
	}
	else if (pBestListNode->front() == WAY_DOWN)
	{
		m_eCurState = Walk_down;
		m_iWay = WAY_DOWN;
	}
	else if (pBestListNode->front() == WAY_UP)
	{
		m_eCurState = Walk_up;
		m_iWay = WAY_UP;
	}
	else if (pBestListNode->front() == WAY_RIGTHUP)
	{
		m_eCurState = Walk_RU;
		m_iWay = WAY_RIGTHUP;
	}
	else if (pBestListNode->front() == WAY_LEFTUP)
	{
		m_eCurState = Walk_LU;
		m_iWay = WAY_LEFTUP;
	}
}

void CPlayer::DrawPlayer()
{
	// 오른팔 0번
	PLAYER* m_PlayerRightArmy = new PLAYER;
	m_PlayerRightArmy->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_PlayerRightArmy->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_PlayerRightArmy->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_PlayerRightArmy->byDrawID = 0;
	m_PlayerRightArmy->wstrStateKey = L"Stand1";
	m_PlayerRightArmy->wstrObjectKey = L"Player_rightArm";
	vecPlayer.push_back(m_PlayerRightArmy);

	// 오른손 1번
	PLAYER* m_PlayerRightArm = new PLAYER;
	m_PlayerRightArm->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_PlayerRightArm->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_PlayerRightArm->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_PlayerRightArm->byDrawID = 0;
	m_PlayerRightArm->wstrStateKey = L"Stand1";
	m_PlayerRightArm->wstrObjectKey = L"Player_rightArmy";
	vecPlayer.push_back(m_PlayerRightArm);

	// 다리 2번
	PLAYER* m_Playerleg = new PLAYER;
	m_Playerleg->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_Playerleg->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Playerleg->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_Playerleg->byDrawID = 0;
	m_Playerleg->wstrStateKey = L"Stand1";
	m_Playerleg->wstrObjectKey = L"Player_legs";
	vecPlayer.push_back(m_Playerleg);

	// 상의 3번
	PLAYER* m_Playerbody = new PLAYER;
	m_Playerbody->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_Playerbody->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Playerbody->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_Playerbody->byDrawID = 0;
	m_Playerbody->wstrStateKey = L"Stand1";
	m_Playerbody->wstrObjectKey = L"Player_body";
	vecPlayer.push_back(m_Playerbody);

	// 머리 4번
	PLAYER* m_PlayerHead = new PLAYER;
	m_PlayerHead->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_PlayerHead->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_PlayerHead->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_PlayerHead->byDrawID = 0;
	m_PlayerHead->wstrStateKey = L"Stand1";
	m_PlayerHead->wstrObjectKey = L"Player_Head";
	vecPlayer.push_back(m_PlayerHead);

	// 왼쪽 팔 5번
	PLAYER* m_PlayerLeftArmy = new PLAYER;
	m_PlayerLeftArmy->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_PlayerLeftArmy->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_PlayerLeftArmy->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_PlayerLeftArmy->byDrawID = 0;
	m_PlayerLeftArmy->wstrStateKey = L"Stand1";
	m_PlayerLeftArmy->wstrObjectKey = L"Player_leftArmy";
	vecPlayer.push_back(m_PlayerLeftArmy);

	// 왼쪽 손 6번
	PLAYER* m_PlayerLeftArm = new PLAYER;
	m_PlayerLeftArm->vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_PlayerLeftArm->vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_PlayerLeftArm->vSize = D3DXVECTOR3(196.f, 148.f, 0.f);
	m_PlayerLeftArm->byDrawID = 0;
	m_PlayerLeftArm->wstrStateKey = L"Stand1";
	m_PlayerLeftArm->wstrObjectKey = L"Player_leftArm";
	vecPlayer.push_back(m_PlayerLeftArm);
}
bool CPlayer::Picking(const D3DXVECTOR3& vPos)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(vecPlayer[0]->vPos.x,
		vecPlayer[0]->vPos.y - (vecPlayer[0]->vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(vecPlayer[0]->vPos.x + (vecPlayer[0]->vSize.x / 2.f),
			vecPlayer[0]->vPos.y,
			0.f),

		D3DXVECTOR3(vecPlayer[0]->vPos.x,
			vecPlayer[0]->vPos.y + (vecPlayer[0]->vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(vecPlayer[0]->vPos.x - (vecPlayer[0]->vSize.x / 2.f),
			vecPlayer[0]->vPos.y,
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

void CPlayer::BuyItem(TCHAR strkey[MIDDLE_STR], int iOption)
{
	float fX = 128.f;
	float fY = -58.f;
	for (int i = 0; i<10; ++i)
	{
		if (m_Inven[i] == NULL)
		{
			D3DXVECTOR3 vPos = D3DXVECTOR3(620.f, 300.f, 0.f) - D3DXVECTOR3(fX, fY, 0.f);
			m_Inven[i] = new CItem(strkey, iOption, vPos);
			break;
		}
		fX -= 64.f;
		if (i == 4)
		{
			fX = 128.f;
			fY -= 64.f;
		}
	}
}

void CPlayer::EquipItem(CItem* pItem)
{
	//m_Inven[i] = new CItem(strkey);

	m_EquipInven[pItem->GetItemInfo().byOption] = new CItem(pItem->GetItemInfo().wstrStateKey, pItem->GetItemInfo().byOption);
}

void CPlayer::DGKeyInput()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos -= m_vScroll;

	static bool SelectPlayer = FALSE;

	if (KEYPRESS(dwKey, LBUTTON))
	{
		if (SelectPlayer == FALSE)
		{
			SelectPlayer = TRUE;

			const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

			if (AStar->GetTileIndex(vMousePos) == -1)
				return;

			if ((*pVecTile)[AStar->GetTileIndex(vMousePos)]->byOption == 1)
				return;

			if (vMousePos.x > m_tInfo.vPos.x)
			{
				if (vMousePos.y - 2 < m_tInfo.vPos.y && m_tInfo.vPos.y < vMousePos.y + 2)
				{
					m_eCurState = Walk_right;
					m_iWay = WAY_RIGTH;
				}
				else if (vMousePos.y > m_tInfo.vPos.y)
				{
					m_eCurState = Walk_RD;
					m_iWay = WAY_RIGTHDOWN;
				}
				else if (vMousePos.y < m_tInfo.vPos.y)
				{
					m_eCurState = Walk_RU;
					m_iWay = WAY_RIGTHUP;
				}
			}
			else if (vMousePos.x < m_tInfo.vPos.x)
			{
				if (vMousePos.y - 2 < m_tInfo.vPos.y && m_tInfo.vPos.y < vMousePos.y + 2)
				{
					m_eCurState = Walk_left;
					m_iWay = WAY_LEFT;
				}
				else if (vMousePos.y > m_tInfo.vPos.y)
				{
					m_eCurState = Walk_LD;
					m_iWay = WAY_LEFTDOWN;
				}
				else if (vMousePos.y < m_tInfo.vPos.y)
				{
					m_eCurState = Walk_LU;
					m_iWay = WAY_LEFTUP;
				}
			}

			if (vMousePos.x - 2 < m_tInfo.vPos.x && m_tInfo.vPos.x < vMousePos.x + 2)
			{
				if (vMousePos.y > m_tInfo.vPos.y)
				{
					m_eCurState = Walk_down;
					m_iWay = WAY_DOWN;
				}
				else if (vMousePos.y < m_tInfo.vPos.y)
				{
					m_eCurState = Walk_up;
					m_iWay = WAY_UP;
				}
			}

			m_iDGEndPos = vMousePos;

			m_bStopMove = true;
			
			m_tInfo.vDir = m_iDGEndPos - m_tInfo.vPos;

			m_iDGDirPos = m_tInfo.vDir;

		}
	}
	else if (KEYPRESS(dwKey, A))
	{
		if (SelectPlayer == FALSE)
		{
			SelectPlayer = TRUE;
			m_bBattle = true;
			m_bAttackTime = true;
		}
	}
	else if (KEYPRESS(dwKey, S))
	{
		if (SelectPlayer == FALSE)
		{
			SelectPlayer = TRUE;
			m_bBattle = true;
			m_bAttackTime = true;
		}
	}
	else if (KEYPRESS(dwKey, P))
	{
		if (CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTilemap())
			CObjMgr::GetInstance()->FindGameObject<CBackGround>()->SetTilemap(false);
		else
			CObjMgr::GetInstance()->FindGameObject<CBackGround>()->SetTilemap(true);
	}
	else
		SelectPlayer = FALSE;

	m_iDGDirPos = m_iDGEndPos - m_tInfo.vPos;

	fDist = D3DXVec3Length(&m_iDGDirPos);

	if (m_bBattle)
	{
		if (m_bAttackTime)
		{
			if (m_iWay == WAY_UP)
				m_eCurState = Attack_up;
			else if (m_iWay == WAY_DOWN)
				m_eCurState = Attack_down;
			else if (m_iWay == WAY_RIGTH)
				m_eCurState = Attack_right;
			else if (m_iWay == WAY_LEFT)
				m_eCurState = Attack_left;
			else if (m_iWay == WAY_RIGTHUP)
				m_eCurState = Attack_RU;
			else if (m_iWay == WAY_RIGTHDOWN)
				m_eCurState = Attack_RD;
			else if (m_iWay == WAY_LEFTUP)
				m_eCurState = Attack_LU;
			else if (m_iWay == WAY_LEFTDOWN)
				m_eCurState = Attack_LD;
			return;
		}
	}

	if (fDist < 5.f)
	{
		m_bStopMove = false;
		if (m_iWay == WAY_UP)
			m_eCurState = Stand_up;
		else if (m_iWay == WAY_DOWN)
			m_eCurState = Stand_down;
		else if (m_iWay == WAY_RIGTH)
			m_eCurState = Stand_right;
		else if (m_iWay == WAY_LEFT)
			m_eCurState = Stand_left;
		else if (m_iWay == WAY_RIGTHUP)
			m_eCurState = Stand_RU;
		else if (m_iWay == WAY_RIGTHDOWN)
			m_eCurState = Stand_RD;
		else if (m_iWay == WAY_LEFTUP)
			m_eCurState = Stand_LU;
		else if (m_iWay == WAY_LEFTDOWN)
			m_eCurState = Stand_LD;
		return;
	}
	if (m_bStopMove)
	{
		D3DXVec3Normalize(&m_iDGDirPos, &m_iDGDirPos);
		m_tInfo.vPos += m_iDGDirPos * GET_TIME * m_fSpeed;
	}
}