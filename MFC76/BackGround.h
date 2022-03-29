#pragma once

#include "Include.h"

class CMFC76View;
class CBackGround
{
	vector<TILE*>		m_vecTile;
	vector<TILE*>		m_vecBackTile;
	vector<INFO*>		m_vecObject;
	vector<UNIT_DATA*>	m_vecMonster;
	vector<OBJECT*>		m_vecCity;

	list<INFO*>			m_MakeObject;
	list<UNIT_DATA*>	m_MakeMonster;
	list<OBJECT*>		m_MakeCity;

private:
	CMFC76View*			m_pMainView;
	INFO*		m_BackObject;

private:
	BOOL	m_tilemap;
	BOOL	m_Font;

public:
	float	m_fObjectX;
	float	m_fObjectY;
	bool	m_bMakeObject;
	float	m_fIfMakeObX;
	float	m_fIfMakeObY;
	float	m_fIfMakeMSX;
	float	m_fIfMakeMSY;
	int		m_iObjectID;
	int		m_iObjectID2;

public:
	bool	TrueObject(const D3DXVECTOR3& vMousePos);
	bool	TrueMonster(const D3DXVECTOR3& vMousePos);
	void	YSorting();


public:
	CBackGround(void);
	~CBackGround(void);

public:
	HRESULT Initialize(void);
	int     Update(void);
	void	Render(void);
	void	Release(void);

public:
	void	ObjectRender();
	void	IfMakeObject();

	void	BackTileRender();
	void    MinRender();

	void	MonsterRender();
	void	IfMonsterRender();

	void	CityObjectRender();
	void	IfCityMakeObject();
	void    CityRender();

public:
	int  GetTileIndex(const D3DXVECTOR3& vMousePos);
	void TileChange(const D3DXVECTOR3& vMousePos, const int& iDrawID, const int& iOption);
	bool Picking(const D3DXVECTOR3& Mousepoint, const int& iIndex);

public:
	void MakeObject(const D3DXVECTOR3& vMousePos);
	bool ObjectPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex);
	int  GetObjectIndex(const D3DXVECTOR3& vMousePos);
	void ObjectChange(const D3DXVECTOR3& vMousePos);

public:
	void MakeMonster(const D3DXVECTOR3& vMousePos);
	bool MonsterPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex);
	int  GetMonsterIndex(const D3DXVECTOR3& vMousePos);
	void MonsterChange(const D3DXVECTOR3& vMousePos);

public:
	void MakeCityObject(const D3DXVECTOR3& vMousePos);
	bool CityPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex);
	int GetCityObjectIndex(const D3DXVECTOR3& vMousePos);
	void ObjectCityChange(const D3DXVECTOR3& vMousePos);
	bool TrueCityObject(const D3DXVECTOR3& vMousePos);

public:
	vector<TILE*>* GetTile() { return &m_vecTile; }
	vector<TILE*>* GetBackTile() { return &m_vecBackTile; }
	vector<INFO*>* GetBackObject() { return &m_vecObject; }
	vector<UNIT_DATA*>* GetBackMonster() { return &m_vecMonster; }
	vector<OBJECT*>* GetBackCity() { return &m_vecCity; }


	list<UNIT_DATA*>*   GetMakeMonster() { return &m_MakeMonster; }
	list<INFO*>*		GetMakeObject() { return &m_MakeObject; }
	list<OBJECT*>*		GetMakeCity() { return &m_MakeCity; }

public:
	void SetMainView(CMFC76View* pView) { m_pMainView = pView; }
	void SetTileMap(BOOL tilemap) { m_tilemap = tilemap; }
	void SetFont(BOOL Font) { m_Font = Font; }
	void SetObjectID(int ObjectID) { m_iObjectID = ObjectID; }
};
