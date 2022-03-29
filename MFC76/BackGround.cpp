#include "StdAfx.h"
#include "BackGround.h"
#include "Device.h"
#include "MainFrm.h"
#include "TextureMgr.h"
#include "MFC76View.h"
#include "Mouse.h"

CBackGround::CBackGround(void)
	:m_Font(TRUE), m_tilemap(TRUE), m_BackObject(NULL),
	m_fObjectX(0.f), m_fObjectY(0.f), m_bMakeObject(false),
	m_fIfMakeObX(0.f), m_fIfMakeObY(0.f), m_iObjectID(0),
	m_iObjectID2(0), m_fIfMakeMSX(0.f), m_fIfMakeMSY(0.f)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

HRESULT CBackGround::Initialize(void)
{
	for (int i = 0; i<TILEY; ++i)
	{
		for (int j = 0; j<TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float fX = j * TILECX + ((i % 2) * (TILECX / 2.f));
			float fY = i * (TILECY * 0.5f);

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3((float)TILECX, (float)TILECY, 0.f);

			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

int CBackGround::Update(void)
{
	return 0;
}

//메인맵 타일
void CBackGround::Render(void)
{
	D3DXMATRIX matWorld, matTrans, matScale;

	POINT m_ptTileRender;

	m_ptTileRender.x = WINCX / TILECX + 2;
	m_ptTileRender.y = WINCY / (TILECY / 2) + 3;

	for(int i = 0; i < m_ptTileRender.y; ++i)
	{
		for(int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = int((m_pMainView->GetScrollPos(0)) / TILECX);
			int iCullY = int((m_pMainView->GetScrollPos(1)) / (TILECY / 2.f));

			int iIndex = (i + iCullY) * TILEX + (j + iCullX);

			if(iIndex < 0 || TILEX * TILEY <= iIndex)
				break;

			const TEX_INFO* pTileTex =
				CTextureMgr::GetInstance()->GetTexture(L"Texture",
					L"Tile",
					m_vecTile[iIndex]->byDrawID);

			D3DXMatrixTranslation(&matTrans
				, m_vecTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0)
				, m_vecTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1)
				, 0.f);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

			matWorld = matScale * matTrans;

			float fX = CENTER_POS(pTileTex->ImageInfo.Width);
			float fY = CENTER_POS(pTileTex->ImageInfo.Height);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
			);
		}
	}
}

//메인맵 타일맵
void CBackGround::BackTileRender()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	POINT m_ptTileRender;

	m_ptTileRender.x = WINCX / TILECX + 2;
	m_ptTileRender.y = WINCY / (TILECY / 2) + 3;

	for (int i = 0; i < m_ptTileRender.y; ++i)
	{
		for (int j = 0; j < m_ptTileRender.x; ++j)
		{
			int iCullX = int((m_pMainView->GetScrollPos(0)) / TILECX) ;
			int iCullY = int((m_pMainView->GetScrollPos(1)) / (TILECY / 2.f));

			int iIndex = (i + iCullY) * TILEX + (j + iCullX);

			if (iIndex < 0 || TILEX * TILEY <= iIndex)
				break;
			//if (m_Font)
			//{
			//	//##폰트 출력
			//	//RECT rc = { 0, -5, 0, 0 };
			//	wsprintf(szIndexText, L"%d", iIndex);
			//	CDevice::GetInstance()->GetFont()->DrawTextW(
			//		CDevice::GetInstance()->GetSprite(),
			//		szIndexText,
			//		lstrlen(szIndexText),
			//		NULL,
			//		DT_CENTER,
			//		D3DCOLOR_ARGB(255, 0, 0, 255)
			//	);
			//}

			if (m_tilemap)
			{
				const TEX_INFO* pBTileTex =
					CTextureMgr::GetInstance()->GetTexture(L"Texture1",
						L"TILEMAP",
						m_vecTile[iIndex]->byOption);

				D3DXMatrixTranslation(&matTrans
					, m_vecTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0)
					, m_vecTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1)
					, 0.f);

				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

				float fX = CENTER_POS(pBTileTex->ImageInfo.Width);
				float fY = CENTER_POS(pBTileTex->ImageInfo.Height);

				matWorld = matScale * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				CDevice::GetInstance()->GetSprite()->Draw(
					pBTileTex->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255)
				);
			}
		}
	}
}
void CBackGround::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safeDelete(m_vecTile[i]);
	}
	m_vecTile.clear();

	for (size_t i = 0; i < m_vecBackTile.size(); ++i)
	{
		safeDelete(m_vecBackTile[i]);
	}
	m_vecBackTile.clear();

	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		safeDelete(m_vecObject[i]);
	}
	m_vecObject.clear();

	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		safeDelete(m_vecMonster[i]);
	}
	m_vecMonster.clear();
}

//  타일 피킹
bool CBackGround::Picking(const D3DXVECTOR3& Mousepoint, const int& iIndex)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x,
		m_vecTile[iIndex]->vPos.y - TILECY / 2.f,
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f,
		m_vecTile[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x,
		m_vecTile[iIndex]->vPos.y + TILECY / 2.f,
		0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f,
		m_vecTile[iIndex]->vPos.y,
		0.f)
	};


	//벡터의 뺄셈으로 방향벡터를 구하자.
	D3DXVECTOR3 vDirection[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDirection[0].y, 
		-vDirection[0].x,
		0.f),

		D3DXVECTOR3(vDirection[1].y, 
		-vDirection[1].x,
		0.f),

		D3DXVECTOR3(vDirection[2].y, 
		-vDirection[2].x,
		0.f),

		D3DXVECTOR3(vDirection[3].y, 
		-vDirection[3].x,
		0.f)
	};


	//단위벡터로 만들어 주자.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = Mousepoint - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if(fDotResult > 0.f)
			return false;
	}

	return true;
}

int  CBackGround::GetTileIndex(const D3DXVECTOR3& vMousePos)
{
	//타일 검사
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if(Picking(vMousePos, i) )
		{
			return i;
		}
	}

	return -1;
}

void CBackGround::TileChange(const D3DXVECTOR3& vMousePos, const int& iDrawID, const int& iOption)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if(iTileIndex == -1)
		return;

	m_vecTile[iTileIndex]->byOption = iOption;		//갈수 없다.
	m_vecTile[iTileIndex]->byDrawID = iDrawID;
}

// 미니맵
void CBackGround::MinRender()
{
	D3DXMATRIX matTrans, matScale, matWorld;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX ; ++j)
		{
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || TILEX * TILEY <= iIndex)
				break;

			const TEX_INFO* pTileTex = 
				CTextureMgr::GetInstance()->GetTexture(L"Texture",
				L"Tile",
				m_vecTile[iIndex]->byDrawID);

			D3DXMatrixScaling(&matScale, 0.2f, 0.45f, 0.0f);

			D3DXMatrixTranslation(&matTrans
				, m_vecTile[iIndex]->vPos.x * 0.2f // - m_pMainView->GetScrollPos(0)
				, m_vecTile[iIndex]->vPos.y * 0.45f  //- m_pMainView->GetScrollPos(1)
				, 0.f);


			matWorld = matScale * matTrans;

			float fX = CENTER_POS(pTileTex->ImageInfo.Width);
			float fY = CENTER_POS(pTileTex->ImageInfo.Height);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL, 
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
				);
		}
	}

	vector<INFO*>::iterator iter = m_vecObject.begin();

	for (; iter != m_vecObject.end(); ++iter)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture(L"Back",
				L"Object",
				(*iter)->byDrawID);

		D3DXMatrixTranslation(&matTrans
			, (*iter)->vPos.x * 0.2f
			, (*iter)->vPos.y * 0.45f
			, 0.f);

		D3DXMatrixScaling(&matScale, 0.2f, 0.45f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

// 오브젝트

void CBackGround::ObjectRender()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	vector<INFO*>::iterator iter = m_vecObject.begin();

	for ( ; iter != m_vecObject.end() ; ++iter)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture(L"Back",
				L"Object",
				(*iter)->byDrawID);

		D3DXMatrixTranslation(&matTrans
			, (*iter)->vPos.x - m_pMainView->GetScrollPos(0)
			, (*iter)->vPos.y - m_pMainView->GetScrollPos(1)
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

void CBackGround::IfMakeObject()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	list<INFO*>::iterator iter = m_MakeObject.begin();

	for ( ; iter != m_MakeObject.end() ; ++iter)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture(L"Back",
				L"Object",
				(*iter)->byDrawID);

		D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

		m_fObjectX = vMousePos.x;
		m_fObjectY = vMousePos.y;

		D3DXMatrixTranslation(&matTrans
			, m_fObjectX
			, m_fObjectY
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		m_fIfMakeObX = fX;
		m_fIfMakeObY = fY;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255)
		);
	}
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void  CBackGround::MakeObject(const D3DXVECTOR3& vMousePos)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if (iTileIndex == -1)
		return;

	m_bMakeObject = true;

	list<INFO*>::iterator iter = m_MakeObject.begin();

	if (iter == m_MakeObject.end())
		return;

	INFO* m_BackObject = new INFO;
	ZeroMemory(m_BackObject, sizeof(INFO));

	m_BackObject->byDrawID = (*iter)->byDrawID;
	m_BackObject->vPos = vMousePos;
	m_BackObject->vSize.x = m_fIfMakeObX;
	m_BackObject->vSize.y = m_fIfMakeObY;
	lstrcpy(m_BackObject->strName, (*iter)->strName);

	m_vecObject.push_back(m_BackObject);
}

bool CBackGround::ObjectPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecObject[iIndex]->vPos.x,
		m_vecObject[iIndex]->vPos.y - m_vecObject[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecObject[iIndex]->vPos.x + m_vecObject[iIndex]->vSize.x,
		m_vecObject[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3(m_vecObject[iIndex]->vPos.x,
		m_vecObject[iIndex]->vPos.y + m_vecObject[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecObject[iIndex]->vPos.x - m_vecObject[iIndex]->vSize.x,
		m_vecObject[iIndex]->vPos.y,
		0.f)
	};


	//벡터의 뺄셈으로 방향벡터를 구하자.
	D3DXVECTOR3 vDirection[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] =
	{
		D3DXVECTOR3(vDirection[0].y,
		-vDirection[0].x,
		0.f),

		D3DXVECTOR3(vDirection[1].y,
		-vDirection[1].x,
		0.f),

		D3DXVECTOR3(vDirection[2].y,
		-vDirection[2].x,
		0.f),

		D3DXVECTOR3(vDirection[3].y,
		-vDirection[3].x,
		0.f)
	};


	//단위벡터로 만들어 주자.
	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = Mousepoint - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if (fDotResult > 0.f)
			return false;
	}
	return true;
}

int  CBackGround::GetObjectIndex(const D3DXVECTOR3& vMousePos)
{
	//타일 검사
	for (size_t i = 0; i < m_vecObject.size(); ++i)
	{
		if (ObjectPicking(vMousePos, i))
		{
			return i;
		}
	}

	return -1;
}

void CBackGround::ObjectChange(const D3DXVECTOR3& vMousePos)
{
	int iObjectIndex = GetObjectIndex(vMousePos);

	if (iObjectIndex == -1)
		return;

	m_iObjectID2 = iObjectIndex;

	vector<INFO*>::iterator iter = m_vecObject.begin();

	for (int i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}
	m_iObjectID = (*iter)->byDrawID;

	m_MakeObject.pop_front();
	m_MakeObject.push_back((*iter));
	m_vecObject.erase(iter);
}

bool CBackGround::TrueObject(const D3DXVECTOR3& vMousePos)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if (iTileIndex == -1)
		return false;

	if (m_vecTile[iTileIndex]->byOption == 0)
		return true;

	return false;
}

//몬스터
void CBackGround::MonsterRender()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	vector<UNIT_DATA*>::iterator iter = m_vecMonster.begin();

	for (; iter != m_vecMonster.end(); ++iter)
	{
		const TEX_INFO* pMonsterTex =
			CTextureMgr::GetInstance()->GetTexture(((*iter)->wstrObjKey),
												   ((*iter)->wstrStateKey),
												   (*iter)->byDrawID);

		D3DXMatrixTranslation(&matTrans
			, (*iter)->vPos.x - m_pMainView->GetScrollPos(0)
			, (*iter)->vPos.y - m_pMainView->GetScrollPos(1)
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pMonsterTex->ImageInfo.Width);
		float fY = CENTER_POS(pMonsterTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pMonsterTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}
void CBackGround::IfMonsterRender()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	list<UNIT_DATA*>::iterator iter = m_MakeMonster.begin();

	for (; iter != m_MakeMonster.end(); ++iter)
	{
		const TEX_INFO* pMonsterTex =
			CTextureMgr::GetInstance()->GetTexture(((*iter)->wstrObjKey),
												  ((*iter)->wstrStateKey),
								    		      (*iter)->byDrawID);

		if (pMonsterTex == NULL)
		{
			AfxMessageBox(L"몬스터이미지X");
			return;
		}

		D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

		m_fObjectX = vMousePos.x;
		m_fObjectY = vMousePos.y;

		D3DXMatrixTranslation(&matTrans
			, m_fObjectX
			, m_fObjectY
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pMonsterTex->ImageInfo.Width);
		float fY = CENTER_POS(pMonsterTex->ImageInfo.Height);

		m_fIfMakeMSX = fX;
		m_fIfMakeMSY = fY;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pMonsterTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255)
		);
	}
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
void CBackGround::MakeMonster(const D3DXVECTOR3& vMousePos)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if (iTileIndex == -1)
		return;

	list<UNIT_DATA*>::iterator iter = m_MakeMonster.begin();

	if (iter == m_MakeMonster.end())
		return;

	UNIT_DATA* m_BackMonster = new UNIT_DATA;
	ZeroMemory(m_BackMonster, sizeof(UNIT_DATA));

	m_BackMonster->vPos = vMousePos;
	m_BackMonster->vSize.x = m_fIfMakeMSX;
	m_BackMonster->vSize.y = m_fIfMakeMSY;

	m_BackMonster->iAtt = (*iter)->iAtt;
	m_BackMonster->iDef = (*iter)->iDef;
	m_BackMonster->iHp = (*iter)->iHp;
	m_BackMonster->iLevel = (*iter)->iLevel;
	lstrcpy(m_BackMonster->wstrName, (*iter)->wstrName);
	m_BackMonster->byDrawID = (*iter)->byDrawID;

	lstrcpy(m_BackMonster->wstrImagePath, (*iter)->wstrImagePath);
	lstrcpy(m_BackMonster->wstrObjKey, (*iter)->wstrObjKey);
	lstrcpy(m_BackMonster->wstrStateKey, (*iter)->wstrStateKey);

	m_vecMonster.push_back(m_BackMonster);
}
bool CBackGround::MonsterPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecMonster[iIndex]->vPos.x,
		m_vecMonster[iIndex]->vPos.y - m_vecMonster[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecMonster[iIndex]->vPos.x + m_vecMonster[iIndex]->vSize.x,
		m_vecMonster[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3(m_vecMonster[iIndex]->vPos.x,
		m_vecMonster[iIndex]->vPos.y + m_vecMonster[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecMonster[iIndex]->vPos.x - m_vecMonster[iIndex]->vSize.x,
		m_vecMonster[iIndex]->vPos.y,
		0.f)
	};


	//벡터의 뺄셈으로 방향벡터를 구하자.
	D3DXVECTOR3 vDirection[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] =
	{
		D3DXVECTOR3(vDirection[0].y,
		-vDirection[0].x,
		0.f),

		D3DXVECTOR3(vDirection[1].y,
		-vDirection[1].x,
		0.f),

		D3DXVECTOR3(vDirection[2].y,
		-vDirection[2].x,
		0.f),

		D3DXVECTOR3(vDirection[3].y,
		-vDirection[3].x,
		0.f)
	};


	//단위벡터로 만들어 주자.
	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = Mousepoint - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if (fDotResult > 0.f)
			return false;
	}
	return true;
}
int  CBackGround::GetMonsterIndex(const D3DXVECTOR3& vMousePos)
{
	for (size_t i = 0; i < m_vecMonster.size(); ++i)
	{
		if (MonsterPicking(vMousePos, i))
		{
			return i;
		}
	}
	return -1;
}
void CBackGround::MonsterChange(const D3DXVECTOR3& vMousePos)
{
	int iMonsterIndex = GetMonsterIndex(vMousePos);

	if (iMonsterIndex == -1)
		return;

	vector<UNIT_DATA*>::iterator iter = m_vecMonster.begin();

	for (int i = 0; i < iMonsterIndex; ++i)
	{
		++iter;
	}

	m_iObjectID = (*iter)->byDrawID;

	m_MakeMonster.pop_front();
	m_MakeMonster.push_back((*iter));
	m_vecMonster.erase(iter);
}
bool CBackGround::TrueMonster(const D3DXVECTOR3& vMousePos)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if (iTileIndex == -1)
		return false;

	if (m_vecTile[iTileIndex]->byOption == 0)
		return true;

	if (m_vecTile[iTileIndex]->byOption == 3)
		return true;

	return false;
}


void CBackGround::YSorting()
{
	vector<INFO*>::iterator iter_begin = m_vecObject.begin();
	vector<INFO*>::iterator iter_end   = m_vecObject.end();
	sort(m_vecObject.begin(), m_vecObject.end(), YSort<INFO*>);
}


//마을오브젝트(상점)
void CBackGround::CityObjectRender()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	vector<OBJECT*>::iterator iter = m_vecCity.begin();

	for (; iter != m_vecCity.end(); ++iter)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture((*iter)->wstrObjKey,
												   (*iter)->wstrStateKey,
				                                   (*iter)->byDrawID);

		D3DXMatrixTranslation(&matTrans
			, (*iter)->vPos.x - m_pMainView->GetScrollPos(0)
			, (*iter)->vPos.y - m_pMainView->GetScrollPos(1)
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
}

void CBackGround::IfCityMakeObject()
{
	D3DXMATRIX matWorld, matTrans, matScale;

	list<OBJECT*>::iterator iter = m_MakeCity.begin();

	for (; iter != m_MakeCity.end(); ++iter)
	{
		const TEX_INFO* pObjectTex =
			CTextureMgr::GetInstance()->GetTexture((*iter)->wstrObjKey,
												   (*iter)->wstrStateKey,
				                                   (*iter)->byDrawID);

		D3DXVECTOR3 vMousePos = CMouse::GetMousePos();

		m_fObjectX = vMousePos.x;
		m_fObjectY = vMousePos.y;

		D3DXMatrixTranslation(&matTrans
			, m_fObjectX
			, m_fObjectY
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		matWorld = matScale * matTrans;

		float fX = CENTER_POS(pObjectTex->ImageInfo.Width);
		float fY = CENTER_POS(pObjectTex->ImageInfo.Height);

		m_fIfMakeObX = fX;
		m_fIfMakeObY = fY;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(
			pObjectTex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255)
		);
	}
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void  CBackGround::MakeCityObject(const D3DXVECTOR3& vMousePos)
{
	//int iTileIndex = GetCityObjectIndex(vMousePos);

	//if (iTileIndex == -1)
	//	return;

	m_bMakeObject = true;

	list<OBJECT*>::iterator iter = m_MakeCity.begin();

	if (iter == m_MakeCity.end())
		return;

	OBJECT* m_BackObject = new OBJECT;
	ZeroMemory(m_BackObject, sizeof(OBJECT));

	m_BackObject->byDrawID = (*iter)->byDrawID;
	m_BackObject->vPos = vMousePos;
	m_BackObject->vSize.x = m_fIfMakeObX;
	m_BackObject->vSize.y = m_fIfMakeObY;
	m_BackObject->byOption = 0;
	lstrcpy(m_BackObject->wstrObjKey, (*iter)->wstrObjKey);
	lstrcpy(m_BackObject->wstrStateKey, (*iter)->wstrStateKey);

	m_vecCity.push_back(m_BackObject);
}

bool CBackGround::CityPicking(const D3DXVECTOR3& Mousepoint, const int& iIndex)
{
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecCity[iIndex]->vPos.x,
		m_vecCity[iIndex]->vPos.y - m_vecCity[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecCity[iIndex]->vPos.x + m_vecCity[iIndex]->vSize.x,
		m_vecCity[iIndex]->vPos.y,
		0.f),

		D3DXVECTOR3(m_vecCity[iIndex]->vPos.x,
		m_vecCity[iIndex]->vPos.y + m_vecCity[iIndex]->vSize.y,
		0.f),

		D3DXVECTOR3(m_vecCity[iIndex]->vPos.x - m_vecCity[iIndex]->vSize.x,
		m_vecCity[iIndex]->vPos.y,
		0.f)
	};


	//벡터의 뺄셈으로 방향벡터를 구하자.
	D3DXVECTOR3 vDirection[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//법선벡터를 만들어 보자.
	D3DXVECTOR3 vNormal[4] =
	{
		D3DXVECTOR3(vDirection[0].y,
		-vDirection[0].x,
		0.f),

		D3DXVECTOR3(vDirection[1].y,
		-vDirection[1].x,
		0.f),

		D3DXVECTOR3(vDirection[2].y,
		-vDirection[2].x,
		0.f),

		D3DXVECTOR3(vDirection[3].y,
		-vDirection[3].x,
		0.f)
	};


	//단위벡터로 만들어 주자.
	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for (int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = Mousepoint - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if (fDotResult > 0.f)
			return false;
	}
	return true;
}

int  CBackGround::GetCityObjectIndex(const D3DXVECTOR3& vMousePos)
{
	//타일 검사
	for (size_t i = 0; i < m_vecCity.size(); ++i)
	{
		if (CityPicking(vMousePos, i))
		{
			return i;
		}
	}

	return -1;
}

void CBackGround::ObjectCityChange(const D3DXVECTOR3& vMousePos)
{
	int iObjectIndex = GetCityObjectIndex(vMousePos);

	if (iObjectIndex == -1)
		return;

	m_iObjectID2 = iObjectIndex;

	vector<OBJECT*>::iterator iter = m_vecCity.begin();

	for (int i = 0; i < iObjectIndex; ++i)
	{
		++iter;
	}
	m_iObjectID = (*iter)->byDrawID;

	m_MakeCity.pop_front();
	m_MakeCity.push_back((*iter));
	m_vecCity.erase(iter);
}

bool CBackGround::TrueCityObject(const D3DXVECTOR3& vMousePos)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if (iTileIndex == -1)
		return false;

	if (m_vecTile[iTileIndex]->byOption == 0)
		return true;

	return false;
}

void CBackGround::CityRender()
{

	D3DXMATRIX matWorld, matTrans, matScale;

	const TEX_INFO* pTileTex =
		CTextureMgr::GetInstance()->GetTexture(L"UI",
			L"TownBack",
			13);

	if (pTileTex == NULL)
	{
		ERROR_MSG(L"백그라운드 이미지 X");
		return;
	}

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	matWorld = matScale;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	float fX = CENTER_POS(pTileTex->ImageInfo.Width);
	float fY = CENTER_POS(pTileTex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTex->pTexture,
		NULL,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
}