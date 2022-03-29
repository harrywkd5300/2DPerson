#pragma once

#include "Include.h"

class CGameObject;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr);

private:
	list<CGameObject*>	m_ObjList[OBJ_TYPE_MAX];
	vector<CGameObject*> m_VecRender[RENDER_TYPE_MAX];

private:
	list<ITEM_DATA*>	m_ListItemData;

public:
	void AddGameObject(CGameObject* pObject, eObjectType eType);

public:
	list<CGameObject*>* GetList() { return m_ObjList; }
	list<CGameObject*>&	GetList(eObjectType eType) { return m_ObjList[eType]; }

	const vector<TILE*>* GetTile();

public:
	template<typename T>
	T* FindGameObject(int iIndex = 0)
	{
		for(size_t i=0 ; i<OBJ_TYPE_MAX ; ++i)
		{
			list<CGameObject*>::iterator iter_begin = m_ObjList[i].begin();
			list<CGameObject*>::iterator iter_end   = m_ObjList[i].end();
			for( ; iter_begin != iter_end ; ++iter_begin)
			{
				if(typeid(T) == typeid( *(*iter_begin) ))
				{
					if(iIndex == 0)
						return dynamic_cast<T*>((*iter_begin));
					else
						--iIndex;
				}
			}
		}
		return NULL;
	}

public:
	HRESULT	Initialize(void);
	int		Update(void);
	void	Render(void);
	void	Release(void);
	void	Release(eObjectType eType);
	void	ReleaseObj(eObjectType eType);

private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
