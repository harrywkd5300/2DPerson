#include "StdAfx.h"
#include "MpBar.h"
#include "UiObserver.h"
#include "DataSubject.h"
#include "Device.h"

CMpBar::CMpBar(void)
{
}

CMpBar::~CMpBar(void)
{
}

HRESULT	CMpBar::Initialize(void)
{
	m_iMp = 0;
	m_tInfo.vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pObserver = new CUiObserver;
	CDataSubject::GetInstance()->AddObserver(m_pObserver);

	return S_OK;
}
int	CMpBar::Update(void)
{
	//m_iMp = dynamic_cast<CUiObserver*>(m_pObserver)->GetUnitData()->iMp;

	D3DXMATRIX matTrans, matParent;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x,
									 m_tInfo.vPos.y,
									 0.f);
	matParent = *dynamic_cast<CUiObserver*>(m_pObserver)->GetMatrix();

	m_tInfo.matWorld = matTrans * matParent;

	return 0;
}
void CMpBar::Render(void)
{
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	TCHAR szMpText[MIDDLE_STR] = L"";

	wsprintf(szMpText, L"HP : %d", m_iMp);

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		szMpText,
		lstrlen(szMpText),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}
void CMpBar::Release(void)
{
	safeDelete(m_pObserver);
}
