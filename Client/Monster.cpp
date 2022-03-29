#include "stdafx.h"
#include "Monster.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include <stdlib.h>
#include "Astar.h"
#include "BackGround.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Player.h"

CMonster::CMonster()
	:m_MoveEnd(true), m_fMoveTime(0.f), m_bMonsterClick(false)
	, iTileIndex(0), m_Astar(false)
	, m_bSetPos(true), m_iIndex(0), m_fAttackTime(0.f), m_bMove(true)
	, m_bEndMove(true), m_bColl(false), m_iCount2(0), m_bColl2(false)
	, m_iCollct(0), m_iNextIndex(0), m_fAddTime(0.f)
{
}

CMonster::~CMonster()
{
	Release();
}

HRESULT	CMonster::Initialize(void)
{
	AStar = new CAstar;
	m_eCurState = Stand_LD;
	m_fSpeed = 100.f;
	m_iCount = 0;
	m_iHp = 500;

	if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	{
		m_vScroll.x = 0.f;
		m_vScroll.y = 0.f;
	}

	iTileIndex = AStar->GetTileIndex(m_tUnit.vPos);
	m_iSelectUnit = 0;

	m_tFrame = FRAME(0.f, 8.f, 8.f);
	m_eRenderID = RENDER_TYPE_OBJECT;

	//m_hdc = GetDC(g_hwnd);
	return S_OK;
}
int	CMonster::Update(void)
{
	m_CollRect.left = int(m_tUnit.vPos.x + m_vScroll.x - 20.f);
	m_CollRect.top = int(m_tUnit.vPos.y + m_vScroll.y - 20.f);
	m_CollRect.right = int(m_tUnit.vPos.x + m_vScroll.x + 20.f);
	m_CollRect.bottom = int(m_tUnit.vPos.y + m_vScroll.y + 20.f);

	if (m_iHp < 0)
	{
		lstrcpy(m_tUnit.wstrStateKey, L"Die");
		m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

		m_tFrame.fMax = 6.f;

		if (m_tFrame.fIndex >= m_tFrame.fMax)
		{
			m_tFrame.fIndex = 0.f;
			return 1;
		}
		return 0;
	}

	if (!m_fAttack)
	{
		m_fAddTime += GET_TIME * 3.f;
		if (m_fAddTime > 6.f)
		{
			m_fAddTime = 0.f;
			m_fAttack = true;
		}
	}

	if (!m_bNotMove)
	{
		Movepattern();

		if (m_bMove)
			AStarMove();

		MyFrameCheck();
		StateChange();
	}
	return 0;
}
void CMonster::Render(void)
{
	D3DXMATRIX matWorld, matTrans, matScale;

	TCHAR szIndexText[MIDDLE_STR] = L"";


	const TEX_INFO* pMonsterTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
											   m_tUnit.wstrStateKey,
											   static_cast<int>(m_tFrame.fIndex));
	if (pMonsterTex == NULL)
	{
		ERROR_MSG(L"몬스터 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 0.f);

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	float fX = CENTER_POS(pMonsterTex->ImageInfo.Width);
	float fY = CENTER_POS(pMonsterTex->ImageInfo.Height) + 33.f;

	CDevice::GetInstance()->GetSprite()->Draw(
		pMonsterTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
	//Ellipse(m_hdc, m_CollRect.left, m_CollRect.top, m_CollRect.right, m_CollRect.bottom);
}
void CMonster::Release(void)
{
	safeDelete(AStar);
}

void CMonster::Movepattern()
{
	if (CSceneMgr::GetInstance()->GetScene() == SCENE_STAGE)
	{
		if (m_MoveEnd)
		{
			m_MoveEnd = false;
			int iX = rand() % 8;

			if (iX == 0)
				vClickMPos = D3DXVECTOR3(m_tUnit.vPos.x -10.f, m_tUnit.vPos.y - 60.f, 0.f);
			else if (iX == 1)
				vClickMPos = D3DXVECTOR3(m_tUnit.vPos.x -10.f, m_tUnit.vPos.y + 40.f, 0.f);
			else if (iX == 2)
				vClickMPos = D3DXVECTOR3(m_tUnit.vPos.x + 40.f, m_tUnit.vPos.y - 10.f, 0.f);
			else if (iX == 3)
				vClickMPos = D3DXVECTOR3(m_tUnit.vPos.x - 60.f, m_tUnit.vPos.y - 10.f, 0.f);
		}

		if (vClickMPos.x < 150.f)
			return;

		m_EndPos = vClickMPos;

		if (m_bAstarNot)
		{
			m_bAstarNot = false;
			AStar->AStarNotStartPos(m_tUnit.vPos, vClickMPos);
		}
	}
	else if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	{
		//int iIndex = AStar->GetTileIndex(m_tUnit.vPos);
		//if (m_iIndex < 0)
		//	return;
		//if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.x > m_tUnit.vPos.x)
		//{
		//	if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
		//	{
		//		//m_iNextIndex = iIndex + (TILEX + (iIndex / TILEX) % 2);
		//		m_eCurState = Walk_RD;
		//		m_iWay = WAY_RIGTHDOWN;
		//	}
		//	else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
		//	{
		//		//m_iNextIndex = iIndex - (TILEX - (iIndex / TILEX) % 2);
		//		m_eCurState = Walk_RU;
		//		m_iWay = WAY_RIGTHUP;
		//	}
		//	else
		//	{
		//		//m_iNextIndex = iIndex + 1;
		//		m_eCurState = Walk_right;
		//		m_iWay = WAY_RIGTH;
		//	}
		//}
		//else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.x < m_tUnit.vPos.x)
		//{
		//	if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
		//	{
		//		//m_iNextIndex = iIndex + (TILEX + (iIndex / TILEX) % 2) - 1;
		//		m_eCurState = Walk_LD;
		//		m_iWay = WAY_LEFTDOWN;
		//	}
		//	else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
		//	{
		//		//m_iNextIndex = iIndex - (TILEX - (iIndex / TILEX) % 2) - 1;
		//		m_eCurState = Walk_LU;
		//		m_iWay = WAY_LEFTUP;
		//	}
		//	else
		//	{
		//		m_eCurState = Walk_left;
		//		m_iWay = WAY_LEFT;
		//	}
		//}
		//else
		//{
		//	if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
		//	{
		//		m_eCurState = Walk_down;
		//		m_iWay = WAY_DOWN;
		//	}
		//	else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
		//	{
		//		m_eCurState = Walk_up;
		//		m_iWay = WAY_UP;
		//	}
		//}
		//const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();
		//int iX = rand() % 8;
		//if ((*CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile())[m_iNextIndex]->byOption == 4)
		//{
		//	(*CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile())[m_iNextIndex]->byOption = 0;
		//	return;
		//}
		//else
		//{
		int iX = rand() % 3;

		if (iX == 0)
			vClickMPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - D3DXVECTOR3(10.f, 50.f, 0.f);
		else if (iX == 1)
			vClickMPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - D3DXVECTOR3(20.f, 10.f, 0.f);
		else if (iX == 2)
			vClickMPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - D3DXVECTOR3(20.f, -30.f, 0.f);
		else if (iX == 3)
			vClickMPos = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - D3DXVECTOR3(20.f, -80.f, 0.f);

		m_tUnit.vDir = vClickMPos - m_tUnit.vPos;

		D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);

		m_tUnit.vPos += m_tUnit.vDir * GET_TIME * m_fSpeed;

		if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.x > m_tUnit.vPos.x)
		{
			if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
			{
				m_eCurState = Walk_RD;
				m_iWay = WAY_RIGTHDOWN;
			}
			else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
			{
				m_eCurState = Walk_RU;
				m_iWay = WAY_RIGTHUP;
			}
			else
			{
				m_eCurState = Walk_right;
				m_iWay = WAY_RIGTH;
			}
		}
		else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.x < m_tUnit.vPos.x)
		{
			if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
			{
				m_eCurState = Walk_LD;
				m_iWay = WAY_LEFTDOWN;
			}
			else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
			{
				m_eCurState = Walk_LU;
				m_iWay = WAY_LEFTUP;
			}
			else
			{
				m_eCurState = Walk_left;
				m_iWay = WAY_LEFT;
			}
		}
		else
		{
			if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y > m_tUnit.vPos.y)
			{
				m_eCurState = Walk_down;
				m_iWay = WAY_DOWN;
			}
			else if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos.y < m_tUnit.vPos.y)
			{
				m_eCurState = Walk_up;
				m_iWay = WAY_UP;
			}
		}
	}
}

void CMonster::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_bBattle)
	{
		if (m_tFrame.fIndex > m_tFrame.fMax)
		{
			m_bAttackTime = false;
			m_tFrame.fIndex = 0.f;
		}

	}
	else
	{
		if (m_tFrame.fIndex > m_tFrame.fMax)
			m_tFrame.fIndex = 0.f;
	}
}

void CMonster::AStarMove(void)
{
	list<int>* pBestList = AStar->GetBestList();
	list<int>* pBestListNode = AStar->GetBestListNode();

	const vector<TILE*>* pVecTile
		= CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if (pVecTile == NULL)
		return;

	m_tUnit.vDir = vClickMPos - m_tUnit.vPos;

	float fDist2 = D3DXVec3Length(&m_tUnit.vDir);

	if (fDist2 < 80.f)
	{
		if (m_iWay == WAY_UP)
			m_eCurState = Battle_up;
		else if (m_iWay == WAY_DOWN)
			m_eCurState = Battle_down;
		else if (m_iWay == WAY_RIGTH)
			m_eCurState = Battle_right;
		else if (m_iWay == WAY_LEFT)
			m_eCurState = Battle_left;
		else if (m_iWay == WAY_RIGTHUP)
			m_eCurState = Battle_RU;
		else if (m_iWay == WAY_RIGTHDOWN)
			m_eCurState = Battle_RD;
		else if (m_iWay == WAY_LEFTUP)
			m_eCurState = Battle_LU;
		else if (m_iWay == WAY_LEFTDOWN)
			m_eCurState = Battle_LD;
		return;
	}

	if (pBestListNode->empty())
		return;

	iTileIndex = AStar->GetTileIndex(m_tUnit.vPos);

	int iMoveIndex = pBestList->front();

	if ((*pVecTile)[iTileIndex]->byOption == 3)
		m_fSpeed = 50.f;
	else
		m_fSpeed = 100.f;

	//(*pVecTile)[iTileIndex]->byOption = 4;

	m_tUnit.vDir = (*pVecTile)[iMoveIndex]->vPos - m_tUnit.vPos;

	float fDist = D3DXVec3Length(&m_tUnit.vDir);

	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);

	m_tUnit.vPos += m_tUnit.vDir * GET_TIME * m_fSpeed;

	if (fDist < 10.f)
	{
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

		m_bAstarNot = true;

		m_fMoveTime += GET_TIME * 6.f;
		if (m_fMoveTime > 20.f)
		{
			m_fMoveTime = 0.f;
			m_MoveEnd = true;
		}
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

void CMonster::StateChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
			////////////Walk////////////////////////
		case Stand_down:    // down
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_5");
			m_fScaleX = 1.f;
			break;
		case Stand_up:    // up
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_4");
			m_fScaleX = 1.f;
			break;
		case Stand_right:    // right
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_2");
			m_fScaleX = -1.f;
			break;
		case Stand_left: // left
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_2");
			m_fScaleX = 1.f;
			break;
		case Stand_LU: // leftup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_3");
			m_fScaleX = 1.f;
			break;
		case Stand_RU: // rightup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_3");
			m_fScaleX = -1.f;
			break;
		case Stand_LD: // leftdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_1");
			m_fScaleX = 1.f;
			break;
		case Stand_RD: // rightdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Stand_1");
			m_fScaleX = -1.f;
			break;

			////////////Walk////////////////////////
		case Walk_down:    // down
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_5");
			m_fScaleX = 1.f;
			break;
		case Walk_up:    // up
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_4");
			m_fScaleX = 1.f;
			break;
		case Walk_right:    // right
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_2");
			m_fScaleX = -1.f;
			break;
		case Walk_left: // left
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_2");
			m_fScaleX = 1.f;
			break;
		case Walk_LU: // leftup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_3");
			m_fScaleX = 1.f;
			break;
		case Walk_RU: // rightup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_3");
			m_fScaleX = -1.f;
			break;
		case Walk_LD: // leftdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_1");
			m_fScaleX = 1.f;
			break;
		case Walk_RD: // rightdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Walk_1");
			m_fScaleX = -1.f;
			break;

			////////////Battle////////////////////////
		case Battle_down:    // down
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_5");
			m_fScaleX = 1.f;
			break;
		case Battle_up:    // up
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_4");
			m_fScaleX = 1.f;
			break;
		case Battle_right:    // right
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_2");
			m_fScaleX = -1.f;
			break;
		case Battle_left: // left
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_2");
			m_fScaleX = 1.f;
			break;
		case Battle_LU: // leftup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_3");
			m_fScaleX = 1.f;
			break;
		case Battle_RU: // rightup
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_3");
			m_fScaleX = -1.f;
			break;
		case Battle_LD: // leftdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_1");
			m_fScaleX = 1.f;
			break;
		case Battle_RD: // rightdown
			m_tFrame.fIndex = 0;
			m_tFrame.fMax = 8;
			lstrcpy(m_tUnit.wstrStateKey, L"Attack_1");
			m_fScaleX = -1.f;
			break;
		}
	}
	m_ePreState = m_eCurState;
}

bool CMonster::Picking(const D3DXVECTOR3& vPos)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_tUnit.vPos.x,
		m_tUnit.vPos.y - (m_tUnit.vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(m_tUnit.vPos.x + (m_tUnit.vSize.x / 2.f),
			m_tUnit.vPos.y,
			0.f),

		D3DXVECTOR3(m_tUnit.vPos.x,
			m_tUnit.vPos.y + (m_tUnit.vSize.y / 2.f),
			0.f),

		D3DXVECTOR3(m_tUnit.vPos.x - (m_tUnit.vSize.x / 2.f),
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

void CMonster::DGAStarMove()
{
	list<int>* pBestList = AStar->GetBestList();
	list<int>* pBestListNode = AStar->GetBestListNode();

	const vector<TILE*>* pVecTile
		= CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if (pVecTile == NULL)
		return;

	if (m_bBattle)
	{
		if (m_bColl2)
		{
			if (this->m_bAttackTime)
			{
				if (m_AttackPos.x > m_tUnit.vPos.x)
				{
					if (m_AttackPos.y > m_tUnit.vPos.y)
						m_iWay = WAY_RIGTHDOWN;
					else if (m_AttackPos.y < m_tUnit.vPos.y)
						m_iWay = WAY_RIGTHUP;
					else
						m_iWay = WAY_RIGTH;
				}
				else if (m_AttackPos.x < m_tUnit.vPos.x)
				{
					if (m_AttackPos.y > m_tUnit.vPos.y)
						m_iWay = WAY_LEFTDOWN;
					else if (m_AttackPos.y < m_tUnit.vPos.y)
						m_iWay = WAY_LEFTUP;
					else
						m_iWay = WAY_LEFT;
				}
				else if (m_AttackPos.x == m_tUnit.vPos.x)
				{
					if (m_AttackPos.y > m_tUnit.vPos.y)
						m_iWay = WAY_DOWN;
					else if (m_AttackPos.y < m_tUnit.vPos.y)
						m_iWay = WAY_UP;
				}
				if (m_iWay == WAY_UP)
					m_eCurState = Battle_up;
				else if (m_iWay == WAY_DOWN)
					m_eCurState = Battle_down;
				else if (m_iWay == WAY_RIGTH)
					m_eCurState = Battle_right;
				else if (m_iWay == WAY_LEFT)
					m_eCurState = Battle_left;
				else if (m_iWay == WAY_RIGTHUP)
					m_eCurState = Battle_RU;
				else if (m_iWay == WAY_RIGTHDOWN)
					m_eCurState = Battle_RD;
				else if (m_iWay == WAY_LEFTUP)
					m_eCurState = Battle_LU;
				else if (m_iWay == WAY_LEFTDOWN)
					m_eCurState = Battle_LD;
				return;
			}
			m_fAttackTime += GET_TIME * 10.f;
			if (m_fAttackTime > 100.f)
			{
				m_fAttackTime = 0.f;
				m_bAttackTime = true;
			}
			return;
		}
	}

	if (pBestList->empty())
	{
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

	if (pBestListNode->empty())
		return;

	iTileIndex = AStar->GetTileIndex(m_tUnit.vPos);

	int iMoveIndex = pBestList->front();

	if (iTileIndex < 0)
		return;

	if ((*pVecTile)[iTileIndex]->byOption == 3)
		m_fSpeed = 50.f;
	else
		m_fSpeed = 100.f;

	m_tUnit.vDir = (*pVecTile)[iMoveIndex]->vPos - m_tUnit.vPos;

	float fDist = D3DXVec3Length(&m_tUnit.vDir);

	if (iTileIndex != iMoveIndex)
	{
		if (iTileIndex > 0)
			(*pVecTile)[iTileIndex]->byOption = 0;
		iTileIndex = iMoveIndex;
	}

	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);

	m_tUnit.vPos += m_tUnit.vDir * GET_TIME * m_fSpeed;

	if (fDist < 5.f)
	{
		pBestList->pop_front();

		if (!pBestList->empty())
			iMoveIndex = pBestList->front();


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