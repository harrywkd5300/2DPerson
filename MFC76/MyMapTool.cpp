// MyMapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MyMapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC76View.h"
#include "MiniView.h"
#include "BackGround.h"

// CMyMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMyMapTool, CDialog)

CMyMapTool::CMyMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYMAPTOOL, pParent), m_iDrawID(0), m_iBackDrawID(0)
	, m_strPath(_T("")), m_wstrPath(L"")
	, m_Tilemap(FALSE)
	, m_Font(FALSE)
{

}

CMyMapTool::~CMyMapTool()
{
	map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapPngImage.end();

	for (iter; iter != iter_end; ++iter)
	{
		iter->second->Destroy();		//이미지 객체를 파괴하자.
		safeDelete(iter->second);
	}
	if (!m_mapPngImage.empty())
		m_mapPngImage.clear();
}

void CMyMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAKEMAP, m_MapListBox);
	DDX_Control(pDX, IDC_STATIC_MAP, m_Picture);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Check(pDX, IDC_CHECK_TILEMAP, m_Tilemap);
	DDX_Check(pDX, IDC_CHECK_FONT, m_Font);
}


BEGIN_MESSAGE_MAP(CMyMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST_MAKEMAP, &CMyMapTool::OnLbnSelchangeListMakemap)
ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMyMapTool::OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMyMapTool::OnBnClickedButtonLoad)
ON_CBN_SELCHANGE(IDC_COMBO1, &CMyMapTool::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_CHECK_TILEMAP, &CMyMapTool::OnBnClickedCheckTilemap)
ON_BN_CLICKED(IDC_CHECK_FONT, &CMyMapTool::OnBnClickedCheckFont)
END_MESSAGE_MAP()


// CMyMapTool 메시지 처리기입니다.

void CMyMapTool::HorizontalScroll(void)
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_MapListBox.GetDC();
	for (int i = 0; i < m_MapListBox.GetCount(); ++i)
	{
		m_MapListBox.GetText(i, strName);
		Size = pDc->GetTextExtent(strName);
		if (Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}
	m_MapListBox.ReleaseDC(pDc);

	if (m_MapListBox.GetHorizontalExtent() < iCX)
	{
		m_MapListBox.SetHorizontalExtent(iCX);
	}

}

void CMyMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";		

	for (int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		CString strImageName;

		//##[17.07.18_02]
		//파일의 이름만 얻어오는 함수.
		strImageName = PathFindFileName(wstrRelativePath.c_str());


		//확장자를 제거해주는 함수.
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());


		//map컨테이너 관리.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if (iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;

			//PNG이미지를 상대경로를 통해서 이미지를 로드한다.
			pPngImage->Load(wstrRelativePath.c_str());

			//키 : 파일이름(ex.Tile0)   
			m_mapPngImage.insert(make_pair(strImageName, pPngImage));
		}


		m_MapListBox.AddString(strImageName);
	}


	HorizontalScroll();


	CDialog::OnDropFiles(hDropInfo);


	UpdateData(FALSE);
}


void CMyMapTool::OnLbnSelchangeListMakemap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_MapListBox.GetCurSel();

	if (iSelectIndex < 0)
		return;

	m_MapListBox.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);


	int i = 0;

	for (i = 0; i < strName.GetLength(); ++i)
	{
		//문자열을 0번 인덱스부터 검색
		//문자가 숫자인지를 검사해보자. -> 
		if (isdigit(strName[i]) != 0)		//Tile0
		{
			break;
			//함수isdigit()의 반환값이 0이 아닌 수이면 문자가 '숫자'이다.
		}
	}

	//i -> 숫자가 있는곳까지의 Index
	strName.Delete(0, i);
	//0번 인덱스부터 i까지 문자열을 삭제한다.


	//_tstoi() : string to int -> 문자열을 정수형으로 변경하는 함수.
	m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}

void CMyMapTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";


	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		AfxMessageBox(L"저장실패.");
		return;
	}

	DWORD dwByte = 0;

	vector<TILE*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	vector<TILE*>::iterator iter_begin = pVector->begin();
	vector<TILE*>::iterator iter_end = pVector->end();

	int iNum = pVector->size();

	for (size_t i=0 ; i<pVector->size() ; ++i)
	{
		WriteFile(hFile, (*pVector)[i], sizeof(TILE), &dwByte, NULL);
	}
	//AfxMessageBox(L"저장성공!!");
	CloseHandle(hFile);
}


void CMyMapTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog	Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	vector<TILE*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	for (size_t i = 0; i<pVector->size(); ++i)
	{
		safeDelete((*pVector)[i]);
	}
	pVector->clear();

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";


	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	// 파일 개방 실패시 NULL이 아닌 INVALID_HANDLE_VALUE값을 반환.
	if (INVALID_HANDLE_VALUE == hFile)
	{
		AfxMessageBox(L"불러오기실패.");
		return;
	}

	DWORD dwByte = 0;

	while (TRUE)
	{
		TILE* pTileData = new TILE;

		ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

		if (dwByte == 0)
		{
			safeDelete(pTileData);
			UpdateData(FALSE);
			break;
		}
		pVector->push_back(pTileData);
	}
	vector<TILE*>::iterator iter_begin = pVector->begin();
	//int iX = (*iter_begin)->iindexX;
	//int iY = (*iter_begin)->iindexY;
	
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMinView()->Invalidate(FALSE);
	CloseHandle(hFile);
	//AfxMessageBox(L"로드성공!!");
}


BOOL CMyMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ComboBox.AddString(L"기본");
	m_ComboBox.AddString(L"슬로우");
	m_ComboBox.AddString(L"이동불가");
	m_ComboBox.AddString(L"포탈");
	m_ComboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMyMapTool::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strName;

	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(), strName);

	if (strName == L"기본")
		m_iBackDrawID = 0;
	else if(strName == L"슬로우")
		m_iBackDrawID = 3;
	else if (strName == L"이동불가")
		m_iBackDrawID = 1;
	else if (strName == L"포탈")
		m_iBackDrawID = 2;


	//m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}


void CMyMapTool::OnBnClickedCheckTilemap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (((CButton*)GetDlgItem(IDC_CHECK_TILEMAP))->GetCheck())
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->SetTileMap(FALSE);
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->SetTileMap(TRUE);
	}
	UpdateData(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMyMapTool::OnBnClickedCheckFont()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (((CButton*)GetDlgItem(IDC_CHECK_FONT))->GetCheck())
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->SetFont(FALSE);
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->SetFont(TRUE);
	}
	UpdateData(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
