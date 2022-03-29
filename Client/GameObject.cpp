#include "StdAfx.h"
#include "GameObject.h"

D3DXVECTOR3 CGameObject::m_vScroll = D3DXVECTOR3(0.f, 0.f, 0.f);

CGameObject::CGameObject(void)
:m_fSpeed(0.f), m_fAngleX(0.f), m_bClickUnit(false), m_bNotClick(false),
m_bBattle(false), m_bAttackTime(true), m_bSkill(true), m_fSkillTime(0.f)
, m_bAttackTime1(true), m_bAttackTime2(true), m_bAttackTime3(true), m_CollisionCount(0),
m_bNotMove(false), m_bAstarNot(true), m_bIsDead(false), m_fAttack(true), m_iHp(100.f), m_iHitTime(true)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tUnit, sizeof(UNIT_DATA));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CGameObject::~CGameObject(void)
{
}
