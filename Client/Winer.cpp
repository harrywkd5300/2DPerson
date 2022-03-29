#include "stdafx.h"
#include "Winer.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "Player.h"

CWiner::CWiner()
{
}


CWiner::~CWiner()
{
	Release();
}

HRESULT	CWiner::Initialize()
{
	m_tUnit.vSize = D3DXVECTOR3(250.f, 92.f, 0.f);
	m_tUnit.vPos = D3DXVECTOR3(400.f, 150.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"UI");
	lstrcpy(m_tUnit.wstrStateKey, L"Win");
	m_eRenderID = RENDER_TYPE_UI;

	m_tFrame = FRAME(0.f, 8.f, 12.f);

	return S_OK;
}

int	CWiner::Update()
{
	if (m_bIsDead)
		return 1;

	FrameCheck();

	return 0;
}

void	CWiner::Render()
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
		, m_tUnit.vPos.x
		, m_tUnit.vPos.y
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
void CWiner::Release()
{
	list<CGameObject*>::iterator iter = CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).begin();
	CObjMgr::GetInstance()->AddGameObject((*iter), OBJ_TYPE_PLAYER);
	CObjMgr::GetInstance()->GetList(OBJ_TYPE_UNIT).pop_front();

	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetPos(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetStartPos());
	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetPlayerPos(CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetStartPos().x,
																	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->GetStartPos().y);

	CObjMgr::GetInstance()->FindGameObject<CPlayer>()->SetNotMove(false);
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);
}

void CWiner::FrameCheck()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if (m_tFrame.fIndex >= m_tFrame.fMax)
	{
		m_tFrame.fIndex = 0.f;
		m_bIsDead = true;
	}
}