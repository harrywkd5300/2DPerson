// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MiniView.h"

#include "MainFrm.h"

#include "Device.h"
#include "MFC76View.h"
#include "TextureMgr.h"
#include "BackGround.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	if(CDevice::GetInstance()->GetDevice() == NULL)
		return;

	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();


	CDevice::GetInstance()->Render_Begin();

	pBackGround->MinRender();

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.

void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetTimer(0, 100, NULL);
}


void CMiniView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnTimer(nIDEvent);
	Invalidate(FALSE);
}
