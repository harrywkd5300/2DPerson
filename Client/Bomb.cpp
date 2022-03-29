#include "stdafx.h"
#include "Bomb.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CBomb::CBomb()
{
}


CBomb::~CBomb()
{
	Release();
}

HRESULT	CBomb::Initialize(void)
{
	lstrcpy(m_tUnit.wstrObjKey, L"Effect");
	lstrcpy(m_tUnit.wstrStateKey, L"BombEff");

	m_tUnit.vSize = D3DXVECTOR3(134.f, 92.f, 0.f);

	m_tFrame = FRAME(0.f, 29.f, 29.f);	

	m_eRenderID = RENDER_TYPE_OBJECT;

	return S_OK;
}

int		CBomb::Update(void)
{
	if (m_bIsDead)
		return 1;

	MyFrameCheck();

	return 0;
}

void CBomb::Render(void)
{
	const TEX_INFO* pBombTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
											   m_tUnit.wstrStateKey,
											   static_cast<int>(m_tFrame.fIndex));
	if (pBombTex == NULL)
	{
		ERROR_MSG(L"Æø¹ß ÀÌ¹ÌÁö X");
		return;
	}


	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pBombTex->ImageInfo.Width);
	float fY = CENTER_POS(pBombTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pBombTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBomb::Release(void)
{

}

void CBomb::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;
	if (m_tFrame.fIndex > m_tFrame.fMax)
	{
		m_bIsDead = true;
		m_tFrame.fIndex = 0.f;
	}
}