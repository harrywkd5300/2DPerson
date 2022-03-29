#pragma once
#include "GameObject.h"

#include "Item.h"

class CAstar;
class CPlayer:
	public CGameObject
{
	UNIT_DATA pWeapon;
	D3DXVECTOR3 vClickMPos;
	vector<PLAYER*> vecPlayer;
	vector<CGameObject*> vecEmploy;
	FRAME m_tWeapon;
	
	D3DXVECTOR3 m_iStartPos;

	D3DXVECTOR3 m_iDGEndPos;
	D3DXVECTOR3 m_iDGDirPos;

	bool		m_bStopMove;

	CItem*		m_Inven[10];
	CItem*		m_EquipInven[8];
	int		    m_iIndex[8];

private:
	CAstar*		AStar;
	float		m_fScaleX;
	int			m_iWayCount;
	eWayType	m_iWay;
	bool		m_bDWClick;
	bool		m_bViewPlayer;
	int			iTileIndex;
	D3DXVECTOR3 m_EndPos;
	float       m_fWalkLength;
	bool		m_iEquipItem;

	int		   m_iFirstRender;
	int		   m_iChangeC;

	bool	   m_SceneChange;

	float		m_fPlusTime;
	float		m_fAddTime;
	float		fDist;

	float		m_fClickTime = 0.f;
	bool		m_bClick = true;

private:
	bool		m_bOpenInven;

private:
	enum State {
		Stand_up, Stand_down, Stand_right, Stand_left, Stand_RD, Stand_RU, Stand_LD, Stand_LU,
		Walk_up, Walk_down, Walk_right, Walk_left, Walk_RD, Walk_RU, Walk_LD, Walk_LU,
		Attack_up, Attack_down, Attack_right, Attack_left, Attack_RD, Attack_RU, Attack_LD, Attack_LU,
				 End };

private:
	State		m_ePreState;
	State		m_eCurState;

public:
	bool		m_bNotMove;

public:
	vector<CGameObject*>& GetEmploy() { return vecEmploy; }
	
	vector<PLAYER*>& GetPlayer() { return vecPlayer; }

	void SetPlayerImage(int i, const D3DXVECTOR3& _vPos)
	{
		vecPlayer[i]->vPos = _vPos;
	}

	D3DXVECTOR3& GetStartPos() { return m_iStartPos; }

	float GetWalkLength() { return m_fWalkLength; }
	void  SetWalkLength(float length) { m_fWalkLength = length; }
	void  SetSceneChange(bool change) { m_SceneChange = change; }
	void  SetNotMove(bool Move) { m_bNotMove = Move; }

	void BuyItem(TCHAR strkey[MIDDLE_STR], int iOption);

	void EquipState(bool Equip) { m_iEquipItem = Equip; }

	CItem*& GetInven(int i) { return m_Inven[i]; }

	CItem*& GetEquipInven(int i) { return m_EquipInven[i]; }
	int&	GetHaveIndex(int i) { return m_iIndex[i]; }

	void EquipItem(CItem* pItem);
	void SwapItem(int iOption, int i)
	{
		CItem* pItem = m_EquipInven[iOption];
		m_EquipInven[iOption] = m_Inven[i];
		m_Inven[i] = pItem;
	}

	void SwapDragItem(int iOption, int i)
	{
		CItem* pItem = m_Inven[iOption];
		m_Inven[iOption] = m_Inven[i];
		m_Inven[i] = pItem;
	}

	void   SellItem(int i)
	{
		safeDelete(m_Inven[i]);
	}

	void EquipItemOut(int i)
	{
		safeDelete(m_EquipInven[i]);
	}

public:
	void SetOpenInven(bool Open) { m_bOpenInven = Open; }
	bool GetOpenInven()			 { return m_bOpenInven; }

public:
	void DrawPlayer();

public:
	CPlayer(void);
	virtual ~CPlayer(void);

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
	void SetPlayerPos(float fX, float fY);
	bool Picking(const D3DXVECTOR3& vPos);


	void DGKeyInput();

public:
	void SetPlayerView(bool View) {  m_bViewPlayer = View; }
};
