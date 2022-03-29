#include "stdafx.h"
#include "BattleCount.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CBattleCount::CBattleCount()
	: m_bSelectPortait(true), m_fAddTime(0)
	, m_fPlusTime(0.f)
{
}
CBattleCount::~CBattleCount()
{
	Release();
}

HRESULT	CBattleCount::Initialize(void)
{
	lstrcpy(m_tUnit.wstrObjKey, L"UI");
	lstrcpy(m_tUnit.wstrStateKey, L"BattleWait");
	m_tFrame = FRAME(0.f, 8.f, 26.f);
	m_eRenderID = RENDER_TYPE_UI;
	//CSoundMgr::GetInstance()->PlayBGM(L"BattleNum.wav", CSoundMgr::Channel_Eff);

	return S_OK;
}
int	CBattleCount::Update(void)
{
	if (m_bIsDead)
		return 1;

	//if (m_bSelectPortait)
	//{
	//	//CSoundMgr::GetInstance()->PlayBGM(L"Player_select.wav", CSoundMgr::Channel_Player);
	//	//플레이어6초, 이순신7초, 
	//	CSoundMgr::GetInstance()->PlayBGM(L"BattleNum.wav", CSoundMgr::Channel_Eff);
	//	m_fPlusTime = 7.f;
	//	m_bSelectPortait = false;
	//}
	MyFrameCheck();
	return 0;
}
void CBattleCount::Render(void)
{
	const TEX_INFO* pBattleTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
											   m_tUnit.wstrStateKey,
												static_cast<int>(m_tFrame.fIndex));
	if (pBattleTex == NULL)
	{
		ERROR_MSG(L"배틀카운트 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x + m_vScroll.x
		, m_tUnit.vPos.y + m_vScroll.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pBattleTex->ImageInfo.Width);
	float fY = CENTER_POS(pBattleTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pBattleTex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CBattleCount::Release(void)
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Eff);
}
void CBattleCount::MyFrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_tFrame.fIndex > m_tFrame.fMax)
		m_tFrame.fIndex = 0.f;

	//if (!m_bSelectPortait)
	//{
	//	m_fAddTime += GET_TIME * 3.f;
	//	if (m_fAddTime > m_fPlusTime)
	//	{
	//		m_fAddTime = 0.f;
	//		CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Eff);
	//		m_bSelectPortait = true;
	//		m_bIsDead = true;
	//	}
	//}
}
