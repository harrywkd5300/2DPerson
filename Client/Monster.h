#pragma once
#include "GameObject.h"

class CAstar;
class CMonster :
	public CGameObject
{
	CAstar*		AStar;
	D3DXVECTOR3 vClickMPos;
	float		m_fScaleX;
	int			m_iWayCount;
	eWayType	m_iWay;
	bool		m_MoveEnd;
	float		m_fMoveTime;
	float		m_fAttackTime;
	bool		m_bMonsterClick;
	int			m_iCount;
	int			iTileIndex;
	int			m_iNextIndex;

	bool        m_Astar;
	bool		m_bSetPos;
	float		m_fDist;
	int			m_iIndex;
	D3DXVECTOR3 m_EndPos;

	bool		m_bMove;
	bool        m_bEndMove;
	float		m_fAddTime;

private:
	D3DXVECTOR3 m_PlayerPos;


private:
	enum State {
		Stand_up, Stand_down, Stand_right, Stand_left, Stand_RD, Stand_RU, Stand_LD, Stand_LU,
		Walk_up, Walk_down, Walk_right, Walk_left, Walk_RD, Walk_RU, Walk_LD, Walk_LU,
		Battle_up, Battle_down, Battle_right, Battle_left, Battle_RD, Battle_RU, Battle_LD, Battle_LU,
		End
	};

private:
	State		m_ePreState;
	State		m_eCurState;

public:
	int    m_type;
	int			m_iCount2;
	int			m_iCollct;
	bool		m_bColl;
	bool		m_bColl2;

public:
	void AStarMove();
	void DGAStarMove();
	void MyFrameCheck();
	void StateChange();

public:
	bool Picking(const D3DXVECTOR3& vPos);
	RECT& GetLocateRect() { return m_LocateRect; }

public:
	void SetPlayerPos(D3DXVECTOR3& vPos) { m_PlayerPos = vPos; }
	void SetMoveMonster(bool move) { m_bMove = move; }


public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CMonster();
	virtual ~CMonster();

public:
	void Movepattern();
};

