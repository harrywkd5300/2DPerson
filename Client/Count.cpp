#include "stdafx.h"
#include "Count.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Player.h"
#include "Monster.h"
#include "BattleCount.h"
#include "SoundMgr.h"

CCount::CCount()
	:m_fAddCount(0.f)
{
}


CCount::~CCount()
{
	Release();
}

HRESULT	CCount::Initialize(void)
{
	m_tUnit.vSize = D3DXVECTOR3(22.f, 28.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"UI");
	lstrcpy(m_tUnit.wstrStateKey, L"BattleCount");
	CSoundMgr::GetInstance()->PlayBGM(L"BattleNum.wav", CSoundMgr::Channel_Eff);
	m_tUnit.byDrawID = 0;
	m_eRenderID = RENDER_TYPE_UI;
	return S_OK;
}
int	CCount::Update(void)
{
	if (m_bIsDead)
		return 1;

	m_fAddCount += GET_TIME * 5.f;
	if (m_fAddCount > 5.f)
	{
		++m_tUnit.byDrawID;
		m_fAddCount = 0.f;
		if (m_tUnit.byDrawID == 4)
		{
			m_bIsDead = true;
		}
	}

	return 0;
}
void CCount::Render(void)
{
	const TEX_INFO* pBattleTex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
			m_tUnit.wstrStateKey,
			m_tUnit.byDrawID);
	if (pBattleTex == NULL)
	{
		ERROR_MSG(L"배틀숫자 이미지 X");
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
void CCount::Release(void)
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Eff);
	//CObjMgr::GetInstance()->FindGameObject<CBattleCount>()->GetDeadState();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_BATTLE);
	CObjMgr::GetInstance()->FindGameObject<CMonster>()->SetNotMove(false);
}
