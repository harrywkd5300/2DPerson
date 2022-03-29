#include "stdafx.h"
#include "Thunder.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CThunder::CThunder()
{
}


CThunder::~CThunder()
{
}

HRESULT	CThunder::Initialize(void)
{
	lstrcpy(m_tUnit.wstrObjKey, L"Effect");
	lstrcpy(m_tUnit.wstrStateKey, L"Light");

	m_tUnit.vSize = D3DXVECTOR3(164.f, 222.f, 0.f);

	m_tFrame = FRAME(0.f, 20.f, 30.f);

	m_eRenderID = RENDER_TYPE_OBJECT;

	return S_OK;
}

int CThunder::Update(void)
{
	if (m_bIsDead)
		return 1;

	m_CollRect.left   = int(m_tUnit.vPos.x - 25.f);
	m_CollRect.top    = int(m_tUnit.vPos.y - 20.f);
	m_CollRect.right  = int(m_tUnit.vPos.x + 25.f);
	m_CollRect.bottom = int(m_tUnit.vPos.y + 20.f);

	MyFrameCheck();

	return 0;
}

void CThunder::Render(void)
{
	const TEX_INFO* pThunderTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
			m_tUnit.wstrStateKey,
			static_cast<int>(m_tFrame.fIndex));

	if (pThunderTex == NULL)
	{
		ERROR_MSG(L"½ã´õ ÀÌ¹ÌÁö X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pThunderTex->ImageInfo.Width);
	float fY = CENTER_POS(pThunderTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pThunderTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CThunder::Release(void)
{

}

void CThunder::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;
	if (m_tFrame.fIndex >= m_tFrame.fMax)
	{
		m_bIsDead = true;
		m_tFrame.fIndex = 0.f;
	}
}