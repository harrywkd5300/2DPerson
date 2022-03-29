#pragma once
#include "GameObject.h"

class CAstar;
class CTiger :
	public CGameObject
{
	CAstar*		AStar;

	D3DXVECTOR3 m_iDGEndPos;
	D3DXVECTOR3 m_iDGDirPos;
	bool		m_bStopMove;
	float		fDist;

	wstring		m_strObjKey;
	wstring		m_strStateKey;

	float		m_fScaleX;
	int			m_iWayCount;
	eWayType	m_iWay;

	float		m_fMoveTime;
	float		m_fAttackTime;

	int			iTileIndex;

private:
	enum State {
		Stand_up, Stand_down, Stand_right, Stand_left, Stand_RD, Stand_RU, Stand_LD, Stand_LU,
		Walk_up, Walk_down, Walk_right, Walk_left, Walk_RD, Walk_RU, Walk_LD, Walk_LU,
		Attack_up, Attack_down, Attack_right, Attack_left, Attack_RD, Attack_RU, Attack_LD, Attack_LU,
		Skill_up, Skill_down, Skill_right, Skill_left, Skill_RD, Skill_RU, Skill_LD, Skill_LU,
		Dead, End
	};

private:
	State		m_ePreState;
	State		m_eCurState;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void MyFrameCheck();
	void KeyInput();
	void StateChange();
	void AStarMove();

	bool Picking(const D3DXVECTOR3& vPos);

public:
	CTiger();
	virtual ~CTiger();
};

