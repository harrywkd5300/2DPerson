// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MyForm.h"

#include "MainFrm.h"
#include "MFC76View.h"
#include "BackGround.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_MAPTOOL, &CMyForm::OnBnClickedButtonMaptool)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT, &CMyForm::OnBnClickedButtonObject)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT2, &CMyForm::OnBnClickedButtonObject2)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_Font.CreatePointFont(150, L"HY�߰��");

	GetDlgItem(IDC_BUTTON_MAPTOOL)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON_OBJECT)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON_OBJECT2)->SetFont(&m_Font);
	//GetDlgItem(IDC_BUTTON_MAPTOOL)->SetFont(&m_Font);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CMyForm::OnBnClickedButtonMaptool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_MyMapTool.GetSafeHwnd() == NULL)
		m_MyMapTool.Create(IDD_MYMAPTOOL);

	m_MyMapTool.ShowWindow(SW_SHOW);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenMapTool(TRUE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenObjectTool(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetMonsterTool(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void CMyForm::OnBnClickedButtonObject()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_ObjectTool.GetSafeHwnd() == NULL)
		m_ObjectTool.Create(IDD_OBJECTTOOL);

	m_ObjectTool.ShowWindow(SW_SHOW);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenMapTool(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenObjectTool(TRUE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetMonsterTool(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMyForm::OnBnClickedButtonObject2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_MonsterTool.GetSafeHwnd() == NULL)
		m_MonsterTool.Create(IDD_MONSTERTOOL);

	m_MonsterTool.ShowWindow(SW_SHOW);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenMapTool(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetOpenObjectTool(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->SetMonsterTool(TRUE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
