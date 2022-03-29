#include "StdAfx.h"
#include "HpBar.h"
#include "UiObserver.h"
#include "DataSubject.h"
#include "Device.h"

CHpBar::CHpBar(void)
{
}

CHpBar::~CHpBar(void)
{
	Release();
}

HRESULT	CHpBar::Initialize(void)
{
	m_iHp = 0;
	m_tInfo.vPos = D3DXVECTOR3(30.f, 150.f, 0.f);
	m_pObserver = new CUiObserver;
	CDataSubject::GetInstance()->AddObserver(m_pObserver);

	return S_OK;
}
int	CHpBar::Update(void)
{
	m_iHp = dynamic_cast<CUiObserver*>(m_pObserver)->GetUnitData()->iHp;
	
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x,
									 m_tInfo.vPos.y,
									 0.f);

	m_tInfo.matWorld = matTrans;

	return 0;
}
void CHpBar::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	TCHAR szHpText[MIDDLE_STR] = L"";

	wsprintf(szHpText, L"HP : %d", m_iHp);

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		szHpText,
		lstrlen(szHpText),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}
void CHpBar::Release(void)
{
	safeDelete(m_pObserver);
}
