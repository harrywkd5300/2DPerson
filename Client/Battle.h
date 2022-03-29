#pragma once
#include "Scene.h"


class CGameObject;
class CBattle :
	public CScene
{
	vector<INFO*>		m_vecObject;
	int iCount;
	float		m_fAddTime;
	D3DXVECTOR3 vUnitPos[10];
	bool	m_bEnd;

public:
	virtual HRESULT	Initialize();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

public:
	vector<INFO*>*	GetGameObject(void)
	{
		return &m_vecObject;
	}

	void LoadObject();
	int& GetCount() { return iCount; }
public:
	bool CheckCollSphere(CGameObject* pDst, CGameObject* pSrc);
	bool CheckCollRect(CGameObject* pDst, CGameObject* pSrc, int* pMoveX, int* pMoveY);

	void CollisionCheck(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);

 public:
	CBattle();
	virtual ~CBattle();

public:
	D3DXVECTOR3 GetMonsterPos(int iA);
};

