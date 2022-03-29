
// MFC76View.h : CMFC76View 클래스의 인터페이스
//


#pragma once

#include "Include.h"

class CBackGround;
class CMFC76Doc;
class CMFC76View : public CScrollView
{
private:
	CBackGround* m_pBackGround;
	int			 m_iTileNum;
	bool		 m_bOpenMapTool;
	bool		 m_bOpenObjectTool;
	bool		 m_bMonsterTool;
	bool		 m_bClick;

public:
	INFO*		 pObject;


protected: // serialization에서만 만들어집니다.
	CMFC76View();
	DECLARE_DYNCREATE(CMFC76View)

// 특성입니다.
public:
	CMFC76Doc* GetDocument() const;

public:
	CBackGround* GetBackGround() { return m_pBackGround; }

	void SetTileNum(int& tileNum) { m_iTileNum = tileNum; }
	void SetOpenMapTool(bool openMap) { m_bOpenMapTool = openMap; }
	void SetOpenObjectTool(bool openObject) { m_bOpenObjectTool = openObject; }
	void SetMonsterTool(bool openMonster) { m_bMonsterTool = openMonster; }


// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFC76View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFC76View.cpp의 디버그 버전
inline CMFC76Doc* CMFC76View::GetDocument() const
   { return reinterpret_cast<CMFC76Doc*>(m_pDocument); }
#endif

