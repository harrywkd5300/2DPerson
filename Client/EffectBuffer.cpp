#include "StdAfx.h"
#include "EffectBuffer.h"
#include "Player.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "Device.h"


CEffectBuffer::CEffectBuffer(void)
{
}

CEffectBuffer::~CEffectBuffer(void)
{
	Release();
}
void CEffectBuffer::WorldMatrixCheck(INFO& rInfo)
{
	D3DXMATRIX matTrans, matRotZ, matParent;
	m_fRotateAngle -= 20 * GET_TIME;
	//D3DXMatrixRotationZ(&matRotZ, m_fRotateAngle);
	D3DXMatrixTranslation(&matTrans, rInfo.vPos.x, rInfo.vPos.y, rInfo.vPos.z);

	CPlayer* pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();

	matParent = pPlayer->GetInfo()->matWorld;

	rInfo.matWorld = matTrans * matParent;

}
void CEffectBuffer::FrameMove()
{
	m_tFrame.fIndex += GET_TIME * m_tFrame.fCount;

	if(m_tFrame.fIndex > m_tFrame.fMax)
		m_tFrame.fIndex = 0.f;
}

HRESULT	CEffectBuffer::Initialize(void)
{
	m_fTime = 2.5f;

	m_fRotateAngle = 0.f;

	m_wstrStateKey = L"BossMultiAttack";

	m_tFrame = FRAME(0.f, 36.f, 36.f);

	return S_OK;
}
int	CEffectBuffer::Update(INFO& rInfo)
{
	WorldMatrixCheck(rInfo);
	FrameMove();
	m_fTime -= GET_TIME;

	if(m_fTime < 0.f)
		return 1;

	return 0;
}
void CEffectBuffer::Render(void)
{
	//CDevice::GetInstance()->Render_End();
	//CDevice::GetInstance()->Render_Begin();

	//CDevice::GetInstance()->GetDevice()->
	//	SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	//CDevice::GetInstance()->GetDevice()->
	//	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	//CDevice::GetInstance()->GetDevice()->
	//	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	CEffectBridge::Render();
}
void CEffectBuffer::Release(void)
{

}