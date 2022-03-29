#pragma once

#include "Include.h"

class CGameObject
{
protected:
	INFO		m_tInfo;
	UNIT_DATA	m_tUnit;
	FRAME		m_tFrame;
	float		m_fSpeed;
	float		m_fAngleX;
	float		m_fDistX;
	float		m_fDistY;
	float		m_iHp;

	bool	    m_iHitTime;

	D3DXVECTOR3		m_vPoint[4];
	D3DXVECTOR3		m_vOriginal[4];

	D3DXMATRIX  matTrans, matScale, matRotZ;

	bool     m_bClickUnit;
	bool	 m_bNotClick;
	bool	 m_bNotMove;

	bool	 m_bAstarNot;
	bool     m_fAttack;

	HDC		m_hdc;

	RECT    m_clickRect;
	RECT    m_LocateRect;

	RECT	m_CollRect;


	eRenderType	m_eRenderID;

	bool		m_bClick;
	bool		m_bIsDead;
	int			 m_iSelectUnit;
	bool		m_bBattle;
	bool		m_bAttackTime;
	bool		m_bAttackTime1;
	bool		m_bAttackTime2;
	bool		m_bAttackTime3;
	bool		m_bSkill;
	float		m_fSkillTime;

	D3DXVECTOR3 m_AttackPos;
	int m_CollisionCount;
protected:
	static D3DXVECTOR3 m_vScroll;

public:
	CGameObject(void);
	virtual ~CGameObject(void);

public:
	INFO* GetInfo() { return &m_tInfo; }
	RECT& GetRect() { return m_clickRect; }
	RECT& GetLocateRect() { return m_LocateRect; }
	RECT& GetCollRect() { return m_CollRect; }

	FRAME& GetFrame() { return m_tFrame; }

	UNIT_DATA* GetUnitInfo() { return &m_tUnit; }
	int&  GetCollICount() { return m_CollisionCount; }

public:
	void SetInfo(const D3DXVECTOR3& vDir) { m_tInfo.vDir = vDir; }
	void SetUnitInfo(const D3DXVECTOR3& vDir) { m_tUnit.vDir = vDir; }

public:
	void SetInfo(INFO* rInfo) { m_tInfo = *rInfo; }
	void SetClickUnit(bool Unitclick) { m_bClickUnit = Unitclick; }
	void SetNotClickUnit(bool Unitclick) { m_bNotClick = Unitclick; }
	void SetBattle(bool battle) { m_bBattle = battle; }
	void SetCollCount(int iCount) { m_CollisionCount = iCount; }
	bool GetDeadState() { return m_bIsDead; }
	void SetAttackPos(float fX, float fY)
	{
		m_AttackPos.x = fX;
		m_AttackPos.y = fY;
	}
	void SetRenderType(eRenderType type) { m_eRenderID = type; }
	float GetiHp() {  return m_iHp; }
	eRenderType GetRenderType() { return m_eRenderID; }
	void SetNotMove(bool DMove) { m_bNotMove = DMove; }
	void SetAttack(bool att) { m_fAttack = att; }
	void SetHitTime(bool time) { m_iHitTime = time; }


	bool GetHitTime() { return m_iHitTime; }
	bool GetAttack() { return m_fAttack; }
	bool GetClickUnit() { return m_bClickUnit; }
	bool GetBattle() { return m_bBattle; }
	bool GetNotClickUnit() { return m_bNotClick; }

	void  SetiHp(float ft) {  m_iHp -= ft;	}


public:
	void SetPos(const D3DXVECTOR3 _vPos) { m_tInfo.vPos = _vPos; }
	void SetPos(float iX, float iY)
	{
		m_tInfo.vPos.x = iX;
		m_tInfo.vPos.y = iY;
		m_tInfo.vPos.z = 0.f;
	}
	void SetUnitPos(float iX, float iY)
	{
		m_tUnit.vPos.x = iX;
		m_tUnit.vPos.y = iY;
		m_tUnit.vPos.z = 0.f;
	}

	void SetMonster(UNIT_DATA* pData)
	{
		m_tUnit.vPos = pData->vPos;
		m_tUnit.vSize = pData->vSize;
		m_tUnit.byOption = pData->byOption;
		m_tUnit.byDrawID = pData->byDrawID;
		lstrcpy(m_tUnit.wstrName, pData->wstrName);
		lstrcpy(m_tUnit.wstrObjKey, pData->wstrObjKey);
		lstrcpy(m_tUnit.wstrStateKey, pData->wstrStateKey);
	}

	D3DXVECTOR3& GetScroll() { return m_vScroll; }

public:
	virtual HRESULT	Initialize(void)PURE;
	virtual int		Update(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;
};
