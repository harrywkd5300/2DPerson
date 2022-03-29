#pragma once

#include "Include.h"

class CAstar
{
private:
	list<NODE*>		m_OpenList;
	list<NODE*>		m_CloseList;
	list<int>		m_BestListNode;

	list<int>		m_BestList;


	eWayType		m_Way;
	D3DXVECTOR3		m_EndPos;

	bool			NotGo;
	int				m_SelectX;
	int				m_SelectY;

	int				m_Openlistsize;


private:
	int		m_iStartIndex;
	int		m_iEndIndex;

public:
	list<int>* GetBestListNode(void)
	{
		return &m_BestListNode;
	}

	list<int>* GetBestList(void)
	{
		return &m_BestList;
	}

	const D3DXVECTOR3& GetEndPos()
	{
		return m_EndPos;
	}


public:
	void	AStarStartPos(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos);
	void	AStarStart(const int& iStartIndex,
					   const int& iEndIndex);

	void	MakeRoute();

	void	AStarNotStartPos(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos);
	void	AStarNotStart(const int& iStartIndex,
					   const int& iEndIndex);

	void	MakeNotRoute();

	NODE*	MakeNode(int iIndex, NODE* pParent, const vector<TILE*>* pTile, eWayType m_Way);

	bool	ListCheck(const int& iIndex);

	void    SetNotGO(bool Trygo) { NotGo = Trygo; }

	void	Release();

public:
	bool Picking(const D3DXVECTOR3& vPos, const TILE* pTile);
	int  GetTileIndex(const D3DXVECTOR3& vPos);

public:
	//[17.08.03_00] : 비용검사를 위한 함수.
	static bool Compare(const NODE* pNode1, const NODE* pNode2);

public:
	CAstar(void);
	~CAstar(void);
};
