#pragma once


class CGameObject;

#include "Monster.h"

template<class T>
class CFactory abstract
{
public:
	// Base Object
	static CGameObject* CreateGameObject()
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		return pObject; 
	}

	static CGameObject* CreateGameObject(D3DXVECTOR3 vPos)
	{
		CGameObject* pObject = new T;
		pObject->Initialize()	;
		pObject->SetPos(vPos);

		return pObject; 
	}
	static CGameObject* CreateGameObject(float fx, float fy)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(fx, fy);

		return pObject;
	}

	static CGameObject* CreateGameObject(float fx, float fy, float fz)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetUnitPos(fx, fy);

		return pObject;
	}

	static CGameObject* CreateGameObject(UNIT_DATA* pData)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetMonster(pData);
		return pObject;
	}

	static CGameObject* CreateGameObject(INFO* tInfo)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetInfo(tInfo);
		return pObject;
	}

	static CGameObject* CreateGameObject(UNIT_DATA* pData, D3DXVECTOR3 vPos, int type)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetMonster(pData);
		pObject->SetUnitPos(vPos.x, vPos.y);
		//dynamic_cast<CMonster*>(pObject)->SetPlayerPos(vPlayer);
		dynamic_cast<CMonster*>(pObject)->m_type = type;
		return pObject;
	}

	static CGameObject* CreateEffect(void)
	{
		CGameObject* pEffect = new CEffect;

		CBridge*	pBridge = new T;

		dynamic_cast<CEffect*>(pEffect)->SetBridge(pBridge);

		pEffect->Initialize();
		pBridge->Initialize();

		return pEffect;
	}

};
