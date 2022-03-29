#pragma once
#include "GameObject.h"
class CPortrait :
	public CGameObject
{

	vector<OBJECT*> vecPortrait;
	bool  m_ViewPortrait;
	bool  m_bOpenInven;
	bool  m_bSelectPortait;

	float m_fAddTime;
	float m_fPlusTime;

	float m_fClickTime = 0.f;
	bool  m_bClick = true;

public:
	TCHAR selectUnit[MIDDLE_STR];

public:
	vector<OBJECT*>* GetPortrait()
	{
		return &vecPortrait;
	}


public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void SetPortrait();
	void SelectObject();
	bool Picking(const D3DXVECTOR3& vPos, const OBJECT* pObject);
	int GetCityIndex(const D3DXVECTOR3& vPos);

	void SetPortrait(bool view) { m_ViewPortrait = view; }
	void SetOpenInven(bool open) { m_bOpenInven = open; }

	void SelectPortait();

	//TCHAR& GetUnit() { return selectUnit; }

public:
	CPortrait();
	virtual ~CPortrait();
};

