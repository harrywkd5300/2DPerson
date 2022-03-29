// MyMapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MyMapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC76View.h"
#include "MiniView.h"
#include "BackGround.h"

// CMyMapTool ��ȭ �����Դϴ�.

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
		iter->second->Destroy();		//�̹��� ��ü�� �ı�����.
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


// CMyMapTool �޽��� ó�����Դϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
		//������ �̸��� ������ �Լ�.
		strImageName = PathFindFileName(wstrRelativePath.c_str());


		//Ȯ���ڸ� �������ִ� �Լ�.
		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR());


		//map�����̳� ����.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if (iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;

			//PNG�̹����� ����θ� ���ؼ� �̹����� �ε��Ѵ�.
			pPngImage->Load(wstrRelativePath.c_str());

			//Ű : �����̸�(ex.Tile0)   
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		//���ڿ��� 0�� �ε������� �˻�
		//���ڰ� ���������� �˻��غ���. -> 
		if (isdigit(strName[i]) != 0)		//Tile0
		{
			break;
			//�Լ�isdigit()�� ��ȯ���� 0�� �ƴ� ���̸� ���ڰ� '����'�̴�.
		}
	}

	//i -> ���ڰ� �ִ°������� Index
	strName.Delete(0, i);
	//0�� �ε������� i���� ���ڿ��� �����Ѵ�.


	//_tstoi() : string to int -> ���ڿ��� ���������� �����ϴ� �Լ�.
	m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}

void CMyMapTool::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if (Dlg.DoModal() == IDCANCEL)
		return;

	Dlg.m_ofn.lpstrInitialDir = L"..\\Data";


	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		AfxMessageBox(L"�������.");
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
	//AfxMessageBox(L"���强��!!");
	CloseHandle(hFile);
}


void CMyMapTool::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ���� ���� ���н� NULL�� �ƴ� INVALID_HANDLE_VALUE���� ��ȯ.
	if (INVALID_HANDLE_VALUE == hFile)
	{
		AfxMessageBox(L"�ҷ��������.");
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
	//AfxMessageBox(L"�ε强��!!");
}


BOOL CMyMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ComboBox.AddString(L"�⺻");
	m_ComboBox.AddString(L"���ο�");
	m_ComboBox.AddString(L"�̵��Ұ�");
	m_ComboBox.AddString(L"��Ż");
	m_ComboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMyMapTool::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	CString strName;

	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(), strName);

	if (strName == L"�⺻")
		m_iBackDrawID = 0;
	else if(strName == L"���ο�")
		m_iBackDrawID = 3;
	else if (strName == L"�̵��Ұ�")
		m_iBackDrawID = 1;
	else if (strName == L"��Ż")
		m_iBackDrawID = 2;


	//m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}


void CMyMapTool::OnBnClickedCheckTilemap()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
