#include "stdafx.h"
#include "Damoon.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BackGround.h"
#include "Mouse.h"


CDamoon::CDamoon()
	:m_fScaleX(1.f), m_fMoveTime(0.f), m_fAttackTime(0.f)
{
	m_iDGEndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_iDGDirPos = D3DXVECTOR3(0.f, 0.f, 0.f);
}


CDamoon::~CDamoon()
{
	Release();
}

HRESULT	CDamoon::Initialize(void)
{
	m_tFrame = FRAME(0.f, 8.f, 8.f);

	//m_tUnit.vPos = D3DXVECTOR3(200.f, 200.f, 0.f);
	m_tUnit.vSize = D3DXVECTOR3(90.f, 40.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"Damoon");
	lstrcpy(m_tUnit.wstrStateKey, L"Stand1");

	m_eCurState = Stand_LD;
	m_bClick = false;
	m_fSpeed = 100.f;

	m_CollRect.left = int(m_tUnit.vPos.x + m_vScroll.x - 30);
	m_CollRect.top = int(m_tUnit.vPos.y + m_vScroll.y - 30);
	m_CollRect.right = int(m_tUnit.vPos.x + m_vScroll.x + 30);
	m_CollRect.bottom = int(m_tUnit.vPos.y + m_vScroll.y + 30);


	AStar = new CAstar;
	m_eRenderID = RENDER_TYPE_OBJECT;
	return S_OK;

}

int	CDamoon::Update(void)
{
	if (m_bClickUnit)
		KeyInput();

	//AStarMove();
	StateChange();
	MyFrameCheck();

	m_CollRect.left = int(m_tUnit.vPos.x + m_vScroll.x - 20.f);
	m_CollRect.top = int(m_tUnit.vPos.y + m_vScroll.y - 20.f);
	m_CollRect.right = int(m_tUnit.vPos.x + m_vScroll.x + 20.f);
	m_CollRect.bottom = int(m_tUnit.vPos.y + m_vScroll.y + 20.f);

	return 0;
}

void CDamoon::Render(void)
{
	const TEX_INFO* pUnitTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
			m_tUnit.wstrStateKey,
			static_cast<int>(m_tFrame.fIndex));
	if (pUnitTex == NULL)
	{
		ERROR_MSG(L"다문 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, m_fScaleX, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pUnitTex->ImageInfo.Width);
	float fY = CENTER_POS(pUnitTex->ImageInfo.Height) + 30.f;

	CDevice::GetInstance()->GetSprite()->Draw(
		pUnitTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CDamoon::Release(void)
{
	safeDelete(AStar);
}

void CDamoon::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_bBattle)
	{
		if (m_tFrame.fIndex > m_tFrame.fMax)
		{
			m_bAttackTime = false;
			m_bSkill = false;
			m_tFrame.fIndex = 0.f;
		}

	}
	else
	{
		if (m_tFrame.fIndex > m_tFrame.fMax)
			m_tFrame.fIndex = 0.f;
	}
}
void CDamoon::KeyInput()
{
	DWORD dwKey = CKeyMgr::GetInstance()->Getkey();

	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos -= m_vScroll;

	static bool SelectLeeSoonSin = FALSE;

	if (KEYPRESS(dwKey, LBUTTON))
	{
		if (SelectLeeSoonSin == FALSE)
		{
			SelectLeeSoonSin = TRUE;
			if (AStar->GetTileIndex(vMousePos) != -1)
			{
				const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

				if ((*pVecTile)[AStar->GetTileIndex(vMousePos)]->byOption == 1)
					return;

				if (vMousePos.x > m_tUnit.vPos.x)
				{
					if (vMousePos.y - 2 < m_tUnit.vPos.y && m_tUnit.vPos.y < vMousePos.y + 2)
					{
						m_eCurState = Walk_right;
						m_iWay = WAY_RIGTH;
					}
					else if (vMousePos.y > m_tUnit.vPos.y)
					{
						m_eCurState = Walk_RD;
						m_iWay = WAY_RIGTHDOWN;
					}
					else if (vMousePos.y < m_tUnit.vPos.y)
					{
						m_eCurState = Walk_RU;
						m_iWay = WAY_RIGTHUP;
					}
				}
				else if (vMousePos.x < m_tUnit.vPos.x)
				{
					if (vMousePos.y - 2 < m_tInfo.vPos.y && m_tInfo.vPos.y < vMousePos.y + 2)
					{
						m_eCurState = Walk_left;
						m_iWay = WAY_LEFT;
					}
					else if (vMousePos.y > m_tUnit.vPos.y)
					{
						m_eCurState = Walk_LD;
						m_iWay = WAY_LEFTDOWN;
					}
					else if (vMousePos.y < m_tUnit.vPos.y)
					{
						m_eCurState = Walk_LU;
						m_iWay = WAY_LEFTUP;
					}
				}

				if (vMousePos.x - 2 < m_tUnit.vPos.x && m_tUnit.vPos.x < vMousePos.x + 2)
				{
					if (vMousePos.y > m_tUnit.vPos.y)
					{
						m_eCurState = Walk_down;
						m_iWay = WAY_DOWN;
					}
					else if (vMousePos.y < m_tUnit.vPos.y)
					{
						m_eCurState = Walk_up;
						m_iWay = WAY_UP;
					}
				}
				m_iDGEndPos = vMousePos;

				m_bStopMove = true;

				m_tUnit.vDir = m_iDGEndPos - m_tUnit.vPos;

				m_iDGDirPos = m_tUnit.vDir;
			}
		}
	}
	else if (KEYPRESS(dwKey, A))
	{
		if (SelectLeeSoonSin == FALSE)
		{
			SelectLeeSoonSin = TRUE;
			m_bBattle = true;
			m_bAttackTime = true;
		}
	}
	else if (KEYPRESS(dwKey, S))
	{
		if (SelectLeeSoonSin == FALSE)
		{
			SelectLeeSoonSin = TRUE;
			m_bBattle = true;
			m_bSkill = true;
		}
	}
	else
		SelectLeeSoonSin = FALSE;

	m_iDGDirPos = m_iDGEndPos - m_tUnit.vPos;

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
		m_tUnit.vPos += m_iDGDirPos * GET_TIME * m_fSpeed;
	}
}

void CDamoon::StateChange()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case Stand_up:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand4");
			m_fScaleX = 1.f;
			break;
		case Stand_down:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand5");
			m_fScaleX = 1.f;
			break;
		case Stand_right:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand2");
			m_fScaleX = -1.f;
			break;
		case Stand_left:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand2");
			m_fScaleX = 1.f;
			break;
		case Stand_RU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand3");
			m_fScaleX = -1.f;
			break;
		case Stand_RD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand1");
			m_fScaleX = -1.f;
			break;
		case Stand_LU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand3");
			m_fScaleX = 1.f;
			break;
		case Stand_LD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Stand1");
			m_fScaleX = 1.f;
			break;
		case Walk_up:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move4");
			m_fScaleX = 1.f;
			break;
		case Walk_down:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move5");
			m_fScaleX = 1.f;
			break;
		case Walk_right:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move2");
			m_fScaleX = -1.f;
			break;
		case Walk_left:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move2");
			m_fScaleX = 1.f;
			break;
		case Walk_RU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move3");
			m_fScaleX = -1.f;
			break;
		case Walk_RD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move1");
			m_fScaleX = -1.f;
			break;
		case Walk_LU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move3");
			m_fScaleX = 1.f;
			break;
		case Walk_LD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Move1");
			m_fScaleX = 1.f;
			break;
		case Attack_up:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack4");
			m_fScaleX = 1.f;
			break;
		case Attack_down:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack5");
			m_fScaleX = 1.f;
			break;
		case Attack_right:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack2");
			m_fScaleX = -1.f;
			break;
		case Attack_left:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack2");
			m_fScaleX = 1.f;
			break;
		case Attack_RU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack3");
			m_fScaleX = -1.f;
			break;
		case Attack_RD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack1");
			m_fScaleX = -1.f;
			break;
		case Attack_LU:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack3");
			m_fScaleX = 1.f;
			break;
		case Attack_LD:
			m_tFrame = FRAME(0.f, 8.f, 8.f);
			lstrcpy(m_tUnit.wstrStateKey, L"Attack1");
			m_fScaleX = 1.f;
			break;
		}
	}
	m_ePreState = m_eCurState;
}

void CDamoon::AStarMove()
{
	list<int>* pBestList = AStar->GetBestList();
	list<int>* pBestListNode = AStar->GetBestListNode();

	const vector<TILE*>* pVecTile
		= CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if (pVecTile == NULL)
		return;
	if (m_bClickUnit)
	{
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
			m_fAttackTime += GET_TIME * 10.f;
			if (m_fAttackTime > 11.f)
			{
				m_fAttackTime = 0.f;
				m_bBattle = false;
			}
			return;
		}
	}
	else
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

	if ((*pVecTile)[iTileIndex]->byOption == 3)
		m_fSpeed = 50.f;
	else
		m_fSpeed = 100.f;


	m_tUnit.vDir = (*pVecTile)[iMoveIndex]->vPos - m_tUnit.vPos;

	float fDist = D3DXVec3Length(&m_tUnit.vDir);

	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);

	m_tUnit.vPos += m_tUnit.vDir * GET_TIME * m_fSpeed;

	if (fDist < 5.f)
	{
		pBestList->pop_front();
		pBestListNode->pop_front();
	}

	if (pBestListNode->empty())
	{
		return;
	}

	if (pBestListNode->front() == WAY_RIGTH)
	{
		m_eCurState = Walk_right;
		m_iWay = WAY_RIGTH;
	}
	else if (pBestListNode->front() == WAY_LEFT)
	{
		m_iWay = WAY_LEFT;
		m_eCurState = Walk_left;
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

bool CDamoon::Picking(const D3DXVECTOR3& vPos)
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