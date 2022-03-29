#pragma once
#include "GameObject.h"
class CItem :
	public CGameObject
{
	OBJECT m_tItem;

	eItemType eType;
	D3DXVECTOR3 m_MovePos;

public:
	CItem();
	CItem(TCHAR strkey[MIDDLE_STR]);
	CItem(TCHAR strkey[MIDDLE_STR], int eType);
	CItem(TCHAR strkey[MIDDLE_STR], int eType, const D3DXVECTOR3& _vPos);
	virtual ~CItem();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void ItemPos(D3DXVECTOR3 ItemPos);
	void SetPos(int iIndex);
	bool Picking(const D3DXVECTOR3& vPos);
	void SetMovePos(const D3DXVECTOR3& vPos)
	{
		m_MovePos = vPos;
		m_tItem.vPos -= m_MovePos;
	}

	OBJECT& GetItemInfo() { return m_tItem; }
};

