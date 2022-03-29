#pragma once
#include "GameObject.h"
class CInventory :
	public CGameObject
{
	UNIT_DATA m_ClickData;
	UNIT_DATA m_ImageData;

	RECT    m_InvenRect[10];
	RECT    m_EquipInvenRect;

	//TCHAR    selectWstrstatekey[MIDDLE_STR];

	int		  m_iSelectIndex;

	bool	  m_bClose;
	bool      m_bClickItem;

	int		  m_iClickIndex;
	bool	  m_bHaveItem;
	D3DXVECTOR3 m_FirstPos;


public:
	CInventory();
	virtual ~CInventory();

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void KeyInput();
	bool ObjectPicking(const D3DXVECTOR3& vPos);
	bool InvenPicking(const D3DXVECTOR3& vPos);
	void MakeRect();

	//void SetSelectUnit(TCHAR strkey[MIDDLE_STR])
	//{
	//	lstrcpy(selectWstrstatekey, strkey);
	//	//selectWstrstatekey = wstrstatekey;
	//}
};

