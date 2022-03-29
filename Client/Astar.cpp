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

//���⼭ ���� Ž���� �����Ѵ�.
//1.Index�� �ʻ� ����� Index������?
//2.Ÿ���� �ɼ��� ���� �ִ� Ÿ���ΰ�?
//3.Open Or Close List�� �ִ� ����ΰ�?

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
		//Ž�� : �� 
		iIndex = pFirstNode->iIndex - TILEX * 2;

		// Ÿ��  ������ �������
		if ( 0 > iIndex || TILEX * TILEY < iIndex)
			return;

		if(pFirstNode->iIndex >= TILEX * 2  &&  1 == m_SelectY &&
			ListCheck(iIndex) && 0 == (*pVecTile)[iIndex]->byOption ||
			2 == (*pVecTile)[iIndex]->byOption || 3 == (*pVecTile)[iIndex]->byOption)
		{
			pNode = MakeNode(iIndex, pFirstNode, pVecTile, WAY_UP);
			m_OpenList.push_back(pNode);
		}


		//Ž�� : �Ʒ�
		iIndex = pFirstNode->iIndex + TILEX * 2;

		// Ÿ��  ������ �������
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

		//Ž�� : ����
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

		//Ž�� : ������
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


		//Ž�� : ������ ��
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


		//Ž�� : ������ �Ʒ�
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


		//Ž�� : ���� �Ʒ�
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


		//Ž�� : ���� ��
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
		
		// ����Ʈ�� ��� ���� �ȴ��ų� ���� ���� ��쿡 ���� ����ó��
		if (m_OpenList.empty() || iOpenListSize > (iTileSize / 4.f))
			return;

		//OpenList�� ��� ����� �����Ѵ�.
		m_OpenList.sort( Compare );

		//���� ���� ����� ���(Ÿ����)
		list<NODE*>::iterator iter = m_OpenList.begin();
		pFirstNode = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if( m_iEndIndex ==  pFirstNode->iIndex )
		{
			//BestList ���ϱ�
			while(true)
			{
				m_BestList.push_back( pFirstNode->iIndex );
				pFirstNode = pFirstNode->pParent;

				if(pFirstNode->iIndex == m_iStartIndex)
					break;
			}

			//���Ҹ� ������Ų��.
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


	//�÷��̾� ��ġ���� ������ ����� �Ÿ�.
	float fPlayerCost = D3DXVec3Length(&fDist);

	//����Ÿ�Ͽ������� �������������� �Ÿ�.
	fDist = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	float fEndCost = D3DXVec3Length(&fDist);

	//�÷��̾���ġ���� �������������� �Ÿ�.
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
	//Ÿ���� ��ġ����
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


	//������ �������� ���⺤�͸� ������.
	D3DXVECTOR3 vDirection[4] = 
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};


	//�������͸� ����� ����.
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


	//�������ͷ� ����� ����.
	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3 vTemp = vPos - vPoint[i];

		D3DXVec3Normalize(&vTemp, &vTemp);

		float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);

		//������ ����� ����� Ÿ���� �ܺ��̰�.
		//				������ Ÿ���� �����̴�.

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
		//���⼭ ���� Ž���� �����Ѵ�.
		//1.Index�� �ʻ� ����� Index������?
		//2.Ÿ���� �ɼ��� ���� �ִ� Ÿ���ΰ�?
		//3.Open Or Close List�� �ִ� ����ΰ�?

		//Ž�� : �� 
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


		//Ž�� : �Ʒ�
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

		//Ž�� : ����
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

		//Ž�� : ������
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


		//Ž�� : ������ ��
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


		//Ž�� : ������ �Ʒ�
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


		//Ž�� : ���� �Ʒ�
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


		//Ž�� : ���� ��
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

		//OpenList�� ��� ����� �����Ѵ�.
		m_OpenList.sort( Compare );

		//if (m_OpenList.empty())
		//	return;

		//���� ���� ����� ���(Ÿ����)
		list<NODE*>::iterator iter = m_OpenList.begin();
		pFirstNode = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if (pFirstNode->iIndex == m_iEndIndex)
		{
			//BestList�� ����.
			while (true)
			{
				m_BestList.push_back(pFirstNode->iIndex);
				m_BestListNode.push_back(pFirstNode->iWay);

				//���������������� ������������ Node...
				pFirstNode = pFirstNode->pParent;

				if (pFirstNode->iIndex == m_iStartIndex)
					break;
			}

			//���Ҹ� ������Ų��.
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