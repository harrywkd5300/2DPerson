// MiniView.cpp : ���� �����Դϴ�.
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


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	if(CDevice::GetInstance()->GetDevice() == NULL)
		return;

	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();


	CDevice::GetInstance()->Render_Begin();

	pBackGround->MinRender();

	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView �����Դϴ�.

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


// CMiniView �޽��� ó�����Դϴ�.

void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	SetTimer(0, 100, NULL);
}


void CMiniView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnTimer(nIDEvent);
	Invalidate(FALSE);
}
