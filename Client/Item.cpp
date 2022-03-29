#include "stdafx.h"
#include "Item.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Inventory.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Mouse.h"
#include "SceneMgr.h"


CItem::CItem()
{
}

CItem::CItem(TCHAR strkey[MIDDLE_STR])
{
	lstrcpy(m_tItem.wstrStateKey, strkey);
	lstrcpy(m_tItem.wstrObjKey, L"Item");
	m_tItem.vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	m_tItem.byDrawID = 0;
}

CItem::CItem(TCHAR strkey[MIDDLE_STR], int eType)
{
	lstrcpy(m_tItem.wstrStateKey, strkey);
	lstrcpy(m_tItem.wstrObjKey, L"Item");
	m_tItem.vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	m_tItem.byOption = eType;
	m_tItem.byDrawID = 0;
}

CItem::CItem(TCHAR strkey[MIDDLE_STR], int eType, const D3DXVECTOR3& _vPos)
{
	lstrcpy(m_tItem.wstrStateKey, strkey);
	lstrcpy(m_tItem.wstrObjKey, L"Item");
	m_tItem.vPos = _vPos;
	m_tItem.vSize = D3DXVECTOR3(60.f, 60.f, 0.f);
	m_tItem.byOption = eType;
	m_tItem.byDrawID = 0;
	m_MovePos = D3DXVECTOR3(300.f, 0.f, 0.f);
}

CItem::~CItem()
{
	Release();
}


HRESULT	CItem::Initialize(void)
{
	return S_OK;
}

int		CItem::Update(void)
{
	return 0;
}

void	CItem::Render(void)
{
	const TEX_INFO* pItemTex =
		CTextureMgr::GetInstance()->GetTexture(m_tItem.wstrObjKey,
											   m_tItem.wstrStateKey,
											   m_tItem.byDrawID);
	if (pItemTex == NULL)
	{
		ERROR_MSG(L"아이템 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tItem.vPos.x 
		, m_tItem.vPos.y 
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tItem.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tItem.matWorld);

	float fX = CENTER_POS(pItemTex->ImageInfo.Width);
	float fY = CENTER_POS(pItemTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pItemTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void	CItem::Release(void)
{


}

void CItem::ItemPos(D3DXVECTOR3 ItemPos)
{
	m_tItem.vPos = ItemPos;
}

void CItem::SetPos(int iIndex)
{
	D3DXVECTOR3 vScroll;

	if (CSceneMgr::GetInstance()->GetScene() == SCENE_CITY)
	{
		m_MovePos.x = 0.f;
		m_MovePos.y = 0.f;
		m_MovePos.z = 0.f;
	}
	else
	{
		m_MovePos.x = 300.f;
		m_MovePos.y = 0.f;
		m_MovePos.z = 0.f;
	}

	float fX = 128.f;
	float fY = -58.f;
	for (int i = 0; i <= iIndex; ++i)
	{
		if (i == iIndex)
		{
			D3DXVECTOR3 vPos = D3DXVECTOR3(620.f, 300.f, 0.f) - D3DXVECTOR3(fX, fY, 0.f);

			vPos = vPos - m_MovePos;

			m_tItem.vPos = vPos;
		}
		fX -= 64.f;
		if (i == 4)
		{
			fX = 128.f;
			fY -= 64.f;
		}
	}
}

bool CItem::Picking(const D3DXVECTOR3& vPos)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_tItem.vPos.x,
		m_tItem.vPos.y - (m_tItem.vSize.y / 2.f),
		0.f),

		D3DXVECTOR3(m_tItem.vPos.x + (m_tItem.vSize.x / 2.f),
		m_tItem.vPos.y,
		0.f),

		D3DXVECTOR3(m_tItem.vPos.x,
		m_tItem.vPos.y + (m_tItem.vSize.y / 2.f),
		0.f),

		D3DXVECTOR3(m_tItem.vPos.x - (m_tItem.vSize.x / 2.f),
		m_tItem.vPos.y,
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