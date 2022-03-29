#include "stdafx.h"
#include "MouseClick.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CMouseClick::CMouseClick()
{
}
CMouseClick::~CMouseClick()
{
	Release();
}


HRESULT	CMouseClick::Initialize(void)
{
	m_tUnit.vSize = D3DXVECTOR3(56.f, 42.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"Effect");
	lstrcpy(m_tUnit.wstrStateKey, L"MoveMark");
	m_eRenderID = RENDER_TYPE_MOUSE;

	m_tFrame = FRAME(0.f, 14.f, 14.f);

	return S_OK;
}

int		CMouseClick::Update(void)
{
	if (m_bIsDead)
		return 1;
	FrameCheck();

	return 0;
}

void	CMouseClick::Render(void)
{
	const TEX_INFO* pPortraitTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
			m_tUnit.wstrStateKey,
			static_cast<int>(m_tFrame.fIndex));
	if (pPortraitTex == NULL)
	{
		ERROR_MSG(L"마우스 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pPortraitTex->ImageInfo.Width);
	float fY = CENTER_POS(pPortraitTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pPortraitTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void	CMouseClick::Release(void)
{

}

void CMouseClick::FrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_tFrame.fIndex >= m_tFrame.fMax)
	{
		m_tFrame.fIndex = 0.f;
		m_bIsDead = true;
	}
}