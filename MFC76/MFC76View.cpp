
// MFC76View.cpp : CMFC76View 클래스의 구현
//

#include "stdafx.h"
#include "MFC76.h"

#include "MFC76Doc.h"
#include "MFC76View.h"

#include "Device.h"

#include "Mouse.h"

#include "MainFrm.h"

#include "BackGround.h"
#include "TextureMgr.h"

#include "MiniView.h"
#include "MyForm.h"
#include "MyMapTool.h"
#include "ObjectTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC76View

IMPLEMENT_DYNCREATE(CMFC76View, CScrollView)

BEGIN_MESSAGE_MAP(CMFC76View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC76View::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFC76View 생성/소멸

CMFC76View::CMFC76View()
:m_iTileNum(0), m_bOpenMapTool(FALSE), m_bOpenObjectTool(FALSE), pObject(NULL), m_bMonsterTool(FALSE), m_bClick(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC76View::~CMFC76View()
{
}

BOOL CMFC76View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY / 2) * TILEY));
	return CView::PreCreateWindow(cs);
}

// CMFC76View 그리기

void CMFC76View::OnDraw(CDC* pDC)
{
	CMFC76Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if(CDevice::GetInstance()->GetDevice() == NULL)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	CDevice::GetInstance()->Render_Begin();

	m_pBackGround->Render();
	m_pBackGround->ObjectRender();
	m_pBackGround->BackTileRender();
	m_pBackGround->IfMakeObject();
	m_pBackGround->IfMonsterRender();
	m_pBackGround->MonsterRender();
	//m_pBackGround->CityRender();
	//m_pBackGround->IfCityMakeObject();
	//m_pBackGround->CityObjectRender();

	CDevice::GetInstance()->Render_End();
}


// CMFC76View 인쇄


void CMFC76View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMFC76View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC76View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC76View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC76View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC76View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMFC76View 진단

#ifdef _DEBUG
void CMFC76View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC76View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC76Doc* CMFC76View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC76Doc)));
	return (CMFC76Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC76View 메시지 처리기

void CMFC76View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;
	m_pMainFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 
		0, 
		0, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	float fX = float(rcWindow.right - rcMainView.right);
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	m_pMainFrame->SetWindowPos(
		NULL,	
		0,
		0,
		int(WINCX + fX),
		int(WINCY + fY),
		SWP_NOZORDER
		);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hwnd = m_hWnd;


	if(FAILED(CDevice::GetInstance()->Init3D() ) )
	{
		AfxMessageBox(L"장치 생성 실패!");
	}

	if (FAILED(CTextureMgr::GetInstance()->LoadImagePath(L"../Data/SaveData.txt")))
	{
		AfxMessageBox(L"멀티텍스쳐 이미지 로드 실패!");
	}

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();
	m_pBackGround->SetMainView(this);
}

void CMFC76View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
	D3DXVECTOR3 vMousePos = CMouse::GetMousePos();
	vMousePos.x += GetScrollPos(0);
	vMousePos.y += GetScrollPos(1);
	vMousePos.z = 0.f;

	m_bClick = true;


	CMyMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();

	if (m_bOpenMapTool)
		m_pBackGround->TileChange(vMousePos, pMapTool->m_iDrawID, pMapTool->m_iBackDrawID);
	else if (m_bOpenObjectTool)
	{
		if (((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->bObjectRadio)
		{
			if (m_pBackGround->GetObjectIndex(vMousePos) == -1)
			{
				if (m_pBackGround->TrueObject(vMousePos))
				{
					m_pBackGround->MakeObject(vMousePos);
					int strPath;
					CString strNum;
					strPath = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetCurSel();
					if (strPath < 0)
						return;
					((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetText(strPath, strNum);
					((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_listBoxAdd.AddString(strNum);
				}
			}
			else
			{
				m_pBackGround->ObjectChange(vMousePos);

				int iObjectID = m_pBackGround->m_iObjectID;
				int iObjectID2 = m_pBackGround->m_iObjectID2;
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->UpdateData(TRUE);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.SetCurSel(iObjectID);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_listBoxAdd.DeleteString(iObjectID2);


				int strPath;
				CString strNum;
				strPath = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetCurSel();
				if (strPath < 0)
					return;
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetText(strPath, strNum);

				map<CString, CImage*>::iterator iter;

				iter = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_mapPngImage.find(strNum);

				if (iter == ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_mapPngImage.end())
					return;

				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectImage.SetBitmap(*iter->second);

				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->UpdateData(FALSE);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->Invalidate(FALSE);
			}
		}
		else if(((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->bCityObjectRadio)
		{
			if (m_pBackGround->GetCityObjectIndex(vMousePos) == -1)
			{
				m_pBackGround->MakeCityObject(vMousePos);
				int strPath;
				CString strNum;
				strPath = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetCurSel();
				if (strPath < 0)
					return;
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetText(strPath, strNum);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_listBoxAdd.AddString(strNum);
			}
			else
			{
				m_pBackGround->ObjectCityChange(vMousePos);

				int iObjectID = m_pBackGround->m_iObjectID;
				int iObjectID2 = m_pBackGround->m_iObjectID2;
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->UpdateData(TRUE);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.SetCurSel(iObjectID);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_listBoxAdd.DeleteString(iObjectID2);


				int strPath;
				CString strNum;
				strPath = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetCurSel();
				if (strPath < 0)
					return;
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectCreateList.GetText(strPath, strNum);

				map<CString, CImage*>::iterator iter;

				iter = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_mapPngImage.find(strNum);

				if (iter == ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_mapPngImage.end())
					return;

				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->m_ObjectImage.SetBitmap(*iter->second);

				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->UpdateData(FALSE);
				((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool()->Invalidate(FALSE);
			}
		}
	}
	else if (m_bMonsterTool)
	{
		if (m_pBackGround->GetMonsterIndex(vMousePos) == -1)
		{
			if (m_pBackGround->TrueMonster(vMousePos))
			{
				m_pBackGround->MakeMonster(vMousePos);
			}
		}
		else
		{
			m_pBackGround->MonsterChange(vMousePos);
		}
	}


	((CMainFrame*)AfxGetMainWnd())->GetMinView()->Invalidate(FALSE);

	Invalidate(FALSE);
}

void CMFC76View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);


	if (!m_bClick)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			OnLButtonDown(nFlags, point);
		}
		else
			m_bClick = false;
	}
	//if (m_bOpenObjectTool)
	//{
	//	CObjectTool* pObjectTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetObjectTool();
	//	pObjectTool->UpdateData(TRUE);
	//	pObjectTool->m_fPostionX = float(point.x);
	//	pObjectTool->m_fPostionY = float(point.y);
	//	pObjectTool->m_fPostionZ = 0.f;
	//	pObjectTool->UpdateData(FALSE);
	//	pObjectTool->Invalidate(FALSE);
	//}
}

void CMFC76View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch(nChar)
	{
	case VK_ESCAPE:
		dynamic_cast<CMainFrame*>(AfxGetMainWnd())->DestroyWindow();
		break;
	}
	Invalidate(FALSE);
}
