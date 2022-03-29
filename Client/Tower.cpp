#include "stdafx.h"
#include "Tower.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Item.h"


CTower::CTower()
	:m_iSelectIndex(0), bObjectClick(false)
{
}

CTower::~CTower()
{
	Release();
}

HRESULT	CTower::Initialize(void)
{
	m_eRenderID = RENDER_TYPE_OBJECT;
	return S_OK;
}

int	CTower::Update(void)
{
	SelectObject();

	return 0;
}

void CTower::Render(void)
{
	D3DXMATRIX matWorld, matTrans, matScale;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	const TEX_INFO* pObjectTex =
		CTextureMgr::GetInstance()->GetTexture(L"Back",
			L"Object",
			m_tInfo.byDrawID);

	if (pObjectTex == NULL)
	{
		ERROR_MSG(L"오브젝트 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tInfo.vPos.x + m_vScroll.x
		, m_tInfo.vPos.y + m_vScroll.y
		, 0.f);

	m_tUnit.vPos = m_tInfo.vPos;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
	float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

	m_tUnit.vSize.x = fX;
	m_tUnit.vSize.y = fY;

	CDevice::GetInstance()->GetSprite()->Draw(
		pObjectTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
}
void CTower::Release(void)
{
}

bool CTower::Picking(const D3DXVECTOR3& vPos)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_tInfo.vPos.x,
		m_tInfo.vPos.y - (m_tInfo.vSize.y / 2.f),
		0.f),

		D3DXVECTOR3(m_tInfo.vPos.x + (m_tInfo.vSize.x / 2.f),
		m_tInfo.vPos.y,
		0.f),

		D3DXVECTOR3(m_tInfo.vPos.x,
		m_tInfo.vPos.y + (m_tInfo.vSize.y / 2.f),
		0.f),

		D3DXVECTOR3(m_tInfo.vPos.x - (m_tInfo.vSize.x / 2.f),
		m_tInfo.vPos.y,
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

void CTower::SelectObject()
{
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

	vMousePos -= m_vScroll;

	if (Picking(vMousePos))
	{
		if (m_tInfo.byDrawID == 55)
			m_tInfo.byDrawID = 56;

		D3DXVECTOR3 fDist = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInfo()->vPos - m_tInfo.vPos;
		float fPlayerCost = D3DXVec3Length(&fDist);
		if (fPlayerCost < 130.f && m_tInfo.byDrawID >= 55)
		{
			if (bObjectClick)
			{
				for (int i = 0; i < 10; ++i)
				{
					if(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i) != NULL)
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetInven(i)->SetMovePos(D3DXVECTOR3(0.f, 0.f, 0.f));
				}
				for (int i = 0; i < 8; ++i)
				{
					if (CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i) != NULL)
						CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetEquipInven(i)->SetMovePos(D3DXVECTOR3(0.f, 0.f, 0.f));
				}
				CSceneMgr::GetInstance()->SetChangeScene(SCENE_CITY);
			}
		}
	}
	else
	{
		if (m_tInfo.byDrawID == 56)
			m_tInfo.byDrawID = 55;
	}
}