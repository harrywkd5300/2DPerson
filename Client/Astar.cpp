#include "StdAfx.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "BackGround.h"


CAstar::CAstar(void)
	:m_iStartIndex(0), m_iEndIndex(0), NotGo(true), m_SelectX(0), m_SelectY(0)
	, m_Openlistsize(0)
{
}

CAstar::~CAstar(void)
{
	Release();
}

void CAstar::AStarStartPos(const D3DXVECTOR3& vStartPos
						   , const D3DXVECTOR3& vEndPos )
{
	if (vEndPos.x >= vStartPos.x)
		m_SelectX = 0;
	if (vEndPos.x < vStartPos.x)
		m_SelectX = 1;

	if (vEndPos.y >= vStartPos.y)
		m_SelectY = 0;
	if (vEndPos.y < vStartPos.y)
		m_SelectY = 1;

	m_iStartIndex = GetTileIndex(vStartPos);
	m_iEndIndex   = GetTileIndex(vEndPos);

	AStarStart(m_iStartIndex, m_iEndIndex);
}


void CAstar::AStarStart(const int& iStartIndex,
					   const int& iEndIndex)
{
	if(iStartIndex == iEndIndex)
		return;

	const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	m_iStartIndex = iStartIndex;
	m_iEndIndex = iEndIndex;
	
	if(pVecTile == NULL)
		return;

	if (m_iStartIndex == -1)
		return;
	if (m_iEndIndex == -1)
		return;


	if((*pVecTile)[iEndIndex]->byOption == 1)
		return;

	Release();

	MakeRoute();
}

//여기서 실제 탐색을 시작한다.
//1.Index가 맵상에 존재는 Index인지를?
//2.타일의 옵션이 갈수 있는 타일인가?
//3.Open Or Close List에 있는 노드인가?

void CAstar::MakeRoute()
{
	NODE* pFirstNode = new NODE;
	pFirstNode->iIndex  = m_iStartIndex;
	pFirstNode->pParent = NULL;
	pFirstNode->fCost	= 0.f;

	m_CloseList.push_back(pFirstNode);

	const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();
	
	if(nullptr == pVecTile)
		return;

	NODE* pNode = nullptr;
	_int   iIndex = 0;

	while(true)
	{
		//탐색 : 위 
		iIndex = pFirstNode->iIndex - TILEX * 2;

		// 타일  밖으로 나갈경우
		if ( 0 > iIndex || TILEX * TILEY < iIndex)
			return;

		if(pFirstNode->iIndex >= TILEX * 2  &&  1 == m_SelectY &&
			ListCheck(iIndex) && 0 == (*pVecTile)[iIndex]->byOption ||
			2 == (*pVecTile)[iIndex]->byOption || 3 == (*pVecTile)[iIndex]->byOption)
		{
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, WAY_UP);
			m_OpenList.push_back(pNode);
		}


		//탐색 : 아래
		iIndex = pFirstNode->iIndex + TILEX * 2;

		// 타일  밖으로 나갈경우
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if(pFirstNode->iIndex < (TILEX * TILEY) - (TILEX * 2) &&
			m_SelectY == 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_DOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);

			m_OpenList.push_back(pNode);
		}

		//탐색 : 왼쪽
		iIndex = pFirstNode->iIndex - 1;
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if(pFirstNode->iIndex % TILEX != 0 &&
			m_SelectX == 1 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_LEFT;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFT;

			m_OpenList.push_back(pNode);
		}

		//탐색 : 오른쪽
		iIndex = pFirstNode->iIndex + 1;
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if(pFirstNode->iIndex % TILEX != TILEX - 1 &&
			m_SelectX == 0  &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_RIGTH;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTH;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 오른쪽 위
		iIndex = pFirstNode->iIndex - (TILEX - (pFirstNode->iIndex / TILEX) % 2);
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if (pFirstNode->iIndex >= TILEX - 1 &&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1) &&
			m_SelectX == 0 &&
			m_SelectY == 1 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_RIGTHUP;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTHUP;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 오른쪽 아래
		iIndex = pFirstNode->iIndex + (TILEX + (pFirstNode->iIndex / TILEX) % 2);
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if (pFirstNode->iIndex < (TILEX * TILEY) - TILEX  &&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1)&&
			m_SelectX == 0 &&
			m_SelectY == 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_RIGTHDOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTHDOWN;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 왼쪽 아래
		iIndex = pFirstNode->iIndex + (TILEX + (pFirstNode->iIndex / TILEX) % 2) - 1;
		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if (pFirstNode->iIndex < (TILEX * TILEY) - TILEX  &&
			pFirstNode->iIndex % (TILEX) != 0 &&
			m_SelectX == 1 &&
			m_SelectY == 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_LEFTDOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFTDOWN;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 왼쪽 위
		iIndex = pFirstNode->iIndex - (TILEX - (pFirstNode->iIndex / TILEX) % 2) - 1;

		if (iIndex < 0 || iIndex > TILEX * TILEY)
			return;

		if (pFirstNode->iIndex >= TILEX - 1 &&
			pFirstNode->iIndex % (TILEX) != 0 &&
			m_SelectX == 1 &&
			m_SelectY == 1 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 2 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_LEFTUP;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFTUP;
			m_OpenList.push_back(pNode);
		}

		_int iOpenListSize = m_OpenList.size();
		_int iTileSize = int(((WINCX / TILECX) * (WINCY / (TILECY / 2.f))));
		
		// 리스트에 담긴 값이 안담기거나 많이 담기는 경우에 대한 예외처리
		if (m_OpenList.empty() || iOpenListSize > (iTileSize / 4.f))
			return;

		//OpenList에 담긴 비용을 정렬한다.
		m_OpenList.sort( Compare );

		//가장 적은 비용의 노드(타일을)
		list<NODE*>::iterator iter = m_OpenList.begin();
		pFirstNode = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if( m_iEndIndex ==  pFirstNode->iIndex )
		{
			//BestList 구하기
			while(true)
			{
				m_BestList.push_back( pFirstNode->iIndex );
				pFirstNode = pFirstNode->pParent;

				if(pFirstNode->iIndex == m_iStartIndex)
					break;
			}

			//원소를 반전시킨다.
			m_BestList.reverse();
			break;
		}
	}
}

//m_BestListNode.push_back(pFirstNode->iWay);
//m_BestListNode.reverse();

NODE* CAstar::MakeNode(int iIndex, NODE* pParent, const vector<TILE*>* pTile, eWayType m_Way)
{
	NODE* pNode = new NODE;

	ZeroMemory(pNode, sizeof(NODE));

	pNode->iIndex  = iIndex;
	pNode->pParent = pParent;

	D3DXVECTOR3 fDist = (*pTile)[iIndex]->vPos - (*pTile)[pParent->iIndex]->vPos;


	//플레이어 위치에서 인접한 노드의 거리.
	float fPlayerCost = D3DXVec3Length(&fDist);

	//인접타일에서부터 도착지점까지의 거리.
	fDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	float fEndCost = D3DXVec3Length(&fDist);

	//플레이어위치에서 도착지점까지의 거리.
	pNode->fCost = fPlayerCost + fEndCost;

	pNode->iWay = m_Way;

	return pNode;
}

bool CAstar::ListCheck(const int& iIndex)
{
	for(list<NODE*>::iterator iter = m_OpenList.begin();
		 iter != m_OpenList.end(); ++iter)
	{
		if( ((*iter))->iIndex == iIndex )
			return false;
	}

	for(list<NODE*>::iterator iter = m_CloseList.begin();
		iter != m_CloseList.end(); ++iter)
	{
		if( ((*iter))->iIndex == iIndex )
			return false;
	}
	return TRUE;
}

void CAstar::Release()
{
	for_each(m_OpenList.begin(), m_OpenList.end(), DeleteObj());
	m_OpenList.clear();

	for_each(m_CloseList.begin(), m_CloseList.end(), DeleteObj());
	m_CloseList.clear();

	m_BestList.clear();
	m_BestListNode.clear();
}

bool CAstar::Picking(const D3DXVECTOR3& vPos
					 , const TILE* pTile)
{
	//타일의 위치벡터
	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3(pTile->vPos.x,
		pTile->vPos.y - TILECY / 2.f,
		0.f),

		D3DXVECTOR3(pTile->vPos.x + TILECX / 2.f,
		pTile->vPos.y,
		0.f),

		D3DXVECTOR3(pTile->vPos.x,
		pTile->vPos.y + TILECY / 2.f,
		0.f),

		D3DXVECTOR3(pTile->vPos.x - TILECX / 2.f,
		pTile->vPos.y,
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
		D3DXVECTOR3 vTemp = vPos - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//내적의 결과가 양수면 타일의 외부이고.
		//				음수면 타일의 내부이다.

		if(fDotResult > 0.f)
			return false;
	}

	return true;
}

int CAstar::GetTileIndex(const D3DXVECTOR3& vPos)
{
	const vector<TILE*>* pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if(pVecTile == NULL)
		return -1;

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		if(Picking(vPos, (*pVecTile)[i]))
		{
			return i;
		}
	}

	return -1;
}

bool CAstar::Compare(const NODE* pNode1, const NODE* pNode2)
{
	return pNode1->fCost < pNode2->fCost;
}

void CAstar::MakeNotRoute()
{
	NODE* pFirstNode = new NODE;
	pFirstNode->iIndex = m_iStartIndex;
	pFirstNode->pParent = NULL;
	pFirstNode->fCost = 0.f;

	m_CloseList.push_back(pFirstNode);

	const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	if (pVecTile == NULL)
		return;

	NODE* pNode = NULL;
	int   iIndex = 0;

	while (true)
	{
		//여기서 실제 탐색을 시작한다.
		//1.Index가 맵상에 존재는 Index인지를?
		//2.타일의 옵션이 갈수 있는 타일인가?
		//3.Open Or Close List에 있는 노드인가?

		//탐색 : 위 
		iIndex = pFirstNode->iIndex - TILEX * 2;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex >= TILEX * 2 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_UP;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_UP;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 아래
		iIndex = pFirstNode->iIndex + TILEX * 2;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex < (TILEX * TILEY) - (TILEX * 2) &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_DOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_DOWN;

			m_OpenList.push_back(pNode);
		}

		//탐색 : 왼쪽
		iIndex = pFirstNode->iIndex - 1;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex % TILEX != 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3 )
		{
			m_Way = WAY_LEFT;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFT;

			m_OpenList.push_back(pNode);
		}

		//탐색 : 오른쪽
		iIndex = pFirstNode->iIndex + 1;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex % TILEX != TILEX - 1 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3 )
		{
			m_Way = WAY_RIGTH;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTH;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 오른쪽 위
		iIndex = pFirstNode->iIndex - (TILEX - (pFirstNode->iIndex / TILEX) % 2);
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex >= TILEX - 1 &&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1) &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3 )
		{
			m_Way = WAY_RIGTHUP;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTHUP;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 오른쪽 아래
		iIndex = pFirstNode->iIndex + (TILEX + (pFirstNode->iIndex / TILEX) % 2);
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex < (TILEX * TILEY) - TILEX  &&
			pFirstNode->iIndex % (TILEX) != (TILEX - 1) &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_RIGTHDOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_RIGTHDOWN;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 왼쪽 아래
		iIndex = pFirstNode->iIndex + (TILEX + (pFirstNode->iIndex / TILEX) % 2) - 1;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex < (TILEX * TILEY) - TILEX  &&
			pFirstNode->iIndex % (TILEX) != 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_LEFTDOWN;
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFTDOWN;

			m_OpenList.push_back(pNode);
		}


		//탐색 : 왼쪽 위
		iIndex = pFirstNode->iIndex - (TILEX - (pFirstNode->iIndex / TILEX) % 2) - 1;
		if (iIndex < 0)
			return;

		if (pFirstNode->iIndex >= TILEX - 1 &&
			pFirstNode->iIndex % (TILEX) != 0 &&
			ListCheck(iIndex) &&
			(*pVecTile)[iIndex]->byOption == 0 ||
			(*pVecTile)[iIndex]->byOption == 3)
		{
			m_Way = WAY_LEFTUP;
			if((*pVecTile)[iIndex]->byOption != 4)
				pNode = MakeNode(iIndex, pFirstNode, pVecTile, m_Way);
			pNode->iWay = WAY_LEFTUP;
			m_OpenList.push_back(pNode);
		}

		int iOpenListSize = m_OpenList.size();
		int iMaxTileSize = int(((WINCX / TILECX) * (WINCY / (TILECY / 2.f))));
		if (m_OpenList.empty() || iOpenListSize > (iMaxTileSize / 4.f))
			return;

		//OpenList에 담긴 비용을 정렬한다.
		m_OpenList.sort( Compare );

		//if (m_OpenList.empty())
		//	return;

		//가장 적은 비용의 노드(타일을)
		list<NODE*>::iterator iter = m_OpenList.begin();
		pFirstNode = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if (pFirstNode->iIndex == m_iEndIndex)
		{
			//BestList를 얻어내자.
			while (true)
			{
				m_BestList.push_back(pFirstNode->iIndex);
				m_BestListNode.push_back(pFirstNode->iWay);

				//도착지점에서부터 시작지점까지 Node...
				pFirstNode = pFirstNode->pParent;

				if (pFirstNode->iIndex == m_iStartIndex)
					break;
			}

			//원소를 반전시킨다.
			m_BestList.reverse();
			m_BestListNode.reverse();
			break;
		}
	}

}

void CAstar::AStarNotStartPos(const D3DXVECTOR3& vStartPos
	, const D3DXVECTOR3& vEndPos)
{
	if (vEndPos.x >= vStartPos.x)
		m_SelectX = 0;
	if (vEndPos.x < vStartPos.x)
		m_SelectX = 1;

	if (vEndPos.y >= vStartPos.y)
		m_SelectY = 0;
	if (vEndPos.y < vStartPos.y)
		m_SelectY = 1;

	m_iStartIndex = GetTileIndex(vStartPos);
	m_iEndIndex = GetTileIndex(vEndPos);

	AStarNotStart(m_iStartIndex, m_iEndIndex);
}


void CAstar::AStarNotStart(const int& iStartIndex,
						   const int& iEndIndex)
{
	if (iStartIndex == iEndIndex)
		return;

	const vector<TILE*>* pVecTile = pVecTile = CObjMgr::GetInstance()->FindGameObject<CBackGround>()->GetTile();

	m_iStartIndex = iStartIndex;
	m_iEndIndex = iEndIndex;

	if (pVecTile == NULL)
		return;

	if (m_iStartIndex < 0)
		return;
	if (m_iEndIndex < 0)
		return;


	if ((*pVecTile)[iEndIndex]->byOption == 1)
		return;

	Release();

	MakeNotRoute();
}