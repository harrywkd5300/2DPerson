#include "stdafx.h"
#include "SteminaBar.h"
#include "Device.h"
#include "TextureMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Factory.h"

CSteminaBar::CSteminaBar()
	:m_fScaleXSize(1.f)
{
}


CSteminaBar::~CSteminaBar()
{
}

HRESULT	CSteminaBar::Initialize(void)
{
	m_tUnit.vPos = D3DXVECTOR3(78.f, 523.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"UI");
	lstrcpy(m_tUnit.wstrStateKey, L"Pannel");

	m_tUnit.byDrawID = 2;

	m_eRenderID = RENDER_TYPE_UI;
	return S_OK;
}

int	CSteminaBar::Update(void)
{
	float WalkLength = CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetWalkLength();

	if (WalkLength > 25000.f)
	{
		m_fScaleXSize -= 0.05f;
		CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetWalkLength(0.f);
	}

	return 0;
}

void	CSteminaBar::Render(void)
{
	const TEX_INFO* pMainUITex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
			m_tUnit.wstrStateKey,
			m_tUnit.byDrawID);
	if (pMainUITex == NULL)
	{
		ERROR_MSG(L"StBar ÀÌ¹ÌÁö X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x
		, m_tUnit.vPos.y
		, 0.f);

	D3DXMatrixScaling(&matScale, m_fScaleXSize, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pMainUITex->ImageInfo.Width);
	float fY = CENTER_POS(pMainUITex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pMainUITex->pTexture,
		NULL,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void	CSteminaBar::Release(void)
{
}
