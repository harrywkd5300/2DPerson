#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"
#include "TextureMgr.h"
#include "BackGround.h"
#include "Tower.h"

IMPLEMENT_SINGLETON(CObjMgr);
CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}

void CObjMgr::AddGameObject(CGameObject* pObject, eObjectType eType)
{
	m_ObjList[eType].push_back(pObject);
}

const vector<TILE*>* CObjMgr::GetTile()
{
	return dynamic_cast<CBackGround*>
		(m_ObjList[OBJ_TYPE_BACKGROUND].front())->GetTile();
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;
}

int CObjMgr::Update(void)
{
	for (size_t i=0 ; i<OBJ_TYPE_MAX ; ++i)
	{
		list<CGameObject*>::iterator iter_begin = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end   = m_ObjList[i].end();

		for ( ; iter_begin != iter_end ;)
		{
			if( (*iter_begin)->Update() == 1)
			{
				safeDelete(*iter_begin);
				iter_begin = m_ObjList[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
	return 0;
}
void CObjMgr::Render(void)
{
	list<CGameObject*>	m_ObjRenderList;
	for (size_t i=0 ; i<OBJ_TYPE_MAX ; ++i)
	{
		list<CGameObject*>::iterator iter_begin = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for ( ; iter_begin != iter_end ; ++iter_begin)
		{
			if (RENDER_TYPE_BACKGROUND == (*iter_begin)->GetRenderType())
			{
				(*iter_begin)->Render();
				continue;
			}
			//if (RENDER_TYPE_OBJECT != (*iter_begin)->GetRenderType())
			//{
			//	(*iter_begin)->Render();
			//	continue;
			//}
			// y ¼ÒÆÃ ¸ÔÀÏ °´Ã¼µé¸¸ µû·Î ºÐ·ùÇØµÐ´Ù.
			eRenderType eID = (*iter_begin)->GetRenderType();
			m_VecRender[eID].push_back((*iter_begin));
		}
	}

	sort(m_VecRender[RENDER_TYPE_OBJECT].begin(), m_VecRender[RENDER_TYPE_OBJECT].end(), YMonsterSort<CGameObject*>);

	for (int i = 0; i < RENDER_TYPE_MAX; ++i)
	{
		for (size_t j = 0; j < m_VecRender[i].size(); ++j)
			(m_VecRender[i][j])->Render();

		m_VecRender[i].clear();
	}
	

}
void CObjMgr::Release(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			safeDelete((*iter));
		}
		m_ObjList[i].clear();
	}
}
void CObjMgr::Release(eObjectType eType)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		if(eType == i)
			continue;

		for(iter; iter != iter_end; ++iter)
		{
			safeDelete(*iter);
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::ReleaseObj(eObjectType eType)
{
	for_each(m_ObjList[eType].begin(), m_ObjList[eType].end(), safeDelete<CGameObject*>);
	m_ObjList[eType].clear();	
}
