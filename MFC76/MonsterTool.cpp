// MonsterTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MonsterTool.h"
#include "afxdialogex.h"

#include "TextureMgr.h"
#include "MainFrm.h"
#include "MFC76View.h"
#include "BackGround.h"


// CMonsterTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonsterTool, CDialog)

CMonsterTool::CMonsterTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MONSTERTOOL, pParent)
	, m_strMonsterName(_T(""))
	, m_iAtk(0)
	, m_iDef(0)
	, m_iLevel(1)
	, m_iHp(0)
	, m_strPathName(_T(""))
	, pObject(NULL)
	, iSelectIndex(0)
{

}

CMonsterTool::~CMonsterTool()
{
}

void CMonsterTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strMonsterName);
	DDX_Text(pDX, IDC_EDIT_ATK, m_iAtk);
	DDX_Text(pDX, IDC_EDIT_DEF, m_iDef);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_iLevel);
	DDX_Text(pDX, IDC_EDIT_GOLD, m_iHp);
	DDX_Control(pDX, IDC_LIST_IMAGELIST, m_iImageList);
	DDX_Control(pDX, IDC_LIST_ADDLIST, m_iAddList);
	DDX_Control(pDX, IDC_STATIC_MONSTERIMAGE, m_iMonsterImage);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strPathName);
}


BEGIN_MESSAGE_MAP(CMonsterTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMonsterTool::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMonsterTool::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMonsterTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMonsterTool::OnBnClickedButtonLoad)
	ON_LBN_SELCHANGE(IDC_LIST_ADDLIST, &CMonsterTool::OnLbnSelchangeListAddlist)
	ON_LBN_SELCHANGE(IDC_LIST_IMAGELIST, &CMonsterTool::OnLbnSelchangeListImagelist)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMonsterTool 메시지 처리기입니다.
void CMonsterTool::HorizontalImageScroll(void)
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_iImageList.GetDC();
	for (int i = 0; i < m_iImageList.GetCount(); ++i)
	{
		m_iImageList.GetText(i, strName);
		Size = pDc->GetTextExtent(strName);
		if (Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}
	m_iImageList.ReleaseDC(pDc);

	if (m_iImageList.GetHorizontalExtent() < iCX)
	{
		m_iImageList.SetHorizontalExtent(iCX);
	}
}
void CMonsterTool::HorizontalAddScroll()
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_iAddList.GetDC();
	for (int i = 0; i < m_iAddList.GetCount(); ++i)
	{
		m_iAddList.GetText(i, strName);
		Size = pDc->GetTextExtent(strName);
		if (Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}
	m_iAddList.ReleaseDC(pDc);

	if (m_iAddList.GetHorizontalExtent() < iCX)
	{
		m_iAddList.SetHorizontalExtent(iCX);
	}
}

void CMonsterTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";			//드롭된 파일의 경로를 보관할 변수.
	
	TCHAR szFileName[MIN_STR] = L"";
	
	for (int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);
		std::wstring wstrRelativePath = RelativePath(szFullPath);

		DirectoryInfoExtraction(szFullPath, m_listImagePath);

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
		m_iImageList.AddString(strImageName);
	}
	UpdateData(FALSE);
}


void CMonsterTool::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_iImageList.GetCurSel() == -1)
		return;

	UNIT_DATA* pMonster = new UNIT_DATA;

	ZeroMemory(pMonster, sizeof(UNIT_DATA));

	lstrcpy(pMonster->wstrName, m_strMonsterName);
	pMonster->iAtt	   = m_iAtk;
	pMonster->iDef     = m_iDef;
	pMonster->iHp	   = m_iHp;
	pMonster->iLevel   = m_iLevel;

	pMonster->byIndex = iSelectIndex;
	pMonster->byDrawID = m_iObjectID;

	list<IMAGE_PATH*>::iterator iter_image = m_listImagePath.begin();

	int iIndex2 = m_iImageList.GetCurSel();

	for (int i = 0; i < iIndex2 ; ++i)
		++iter_image;

	lstrcpy(pMonster->wstrImagePath, (*iter_image)->wstrPath.c_str());
	lstrcpy(pMonster->wstrObjKey, (*iter_image)->wstrObjKey.c_str());
	lstrcpy(pMonster->wstrStateKey, (*iter_image)->wstrStateKey.c_str());

	m_listMonster.push_back(pMonster);
	m_iAddList.AddString(pMonster->wstrName);
	UpdateData(FALSE);
}


void CMonsterTool::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iIndex = m_iAddList.GetCurSel();

	if (iIndex == -1)
		return;

	list<UNIT_DATA*>::iterator iter;
	iter = m_listMonster.begin();

	for (int i = 0; i < iIndex; ++i)
		++iter;

	m_listMonster.erase(iter);

	m_iAtk = 0;
	m_iDef = 0;
	m_iLevel = 1;
	m_iHp = 0;

	m_strMonsterName = _T("");
	m_iImageList.SetCurSel(-1);
	m_iAddList.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CMonsterTool::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	wstring FilePath = m_strPathName.operator LPCWSTR();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (FilePath == _T(""))
	{
		MessageBox(L"저장실패!!", L"저장에러!");
		return;
	}
	else
	{
		FilePath = L"../Data/" + FilePath;

		vector<UNIT_DATA*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackMonster();
		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_WRITE, NULL, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		for (size_t i = 0; i < pVector->size(); ++i)
		{
			WriteFile(hFile, (*pVector)[i], sizeof(UNIT_DATA), &dwByte, NULL);
		}
		CloseHandle(hFile);
	}
}


void CMonsterTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	wstring FilePath = m_strPathName.operator LPCWSTR();
	UpdateData(FALSE);
	if (m_strPathName == _T(""))
	{
		MessageBox(L"로드실패!!", L"로드에러!");
		return;
	}
	else
	{
		FilePath = L"../Data/" + FilePath;
		vector<UNIT_DATA*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackMonster();

		DWORD dwByte = 0;

		HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_READ, NULL, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		for (size_t i = 0; i < pVector->size(); ++i)
		{
			safeDelete((*pVector)[i]);
		}
		pVector->clear();

		while (TRUE)
		{
			UNIT_DATA* pObjectData = new UNIT_DATA;
			ZeroMemory(pObjectData, sizeof(UNIT_DATA));

			ReadFile(hFile, pObjectData, sizeof(UNIT_DATA), &dwByte, NULL);

			if (dwByte == 0)
			{
				safeDelete(pObjectData);
				break;
			}

			pVector->push_back(pObjectData);
		}

		CloseHandle(hFile);
	}
}


void CMonsterTool::OnLbnSelchangeListAddlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_iAddList.GetCurSel();

	list<UNIT_DATA*>::iterator iter;
	iter = m_listMonster.begin();

	for (int i = 0; i < iIndex; ++i)
		++iter;

	m_iAtk = (*iter)->iAtt;
	m_iDef = (*iter)->iDef;
	m_iLevel = (*iter)->iLevel;
	m_iHp	 = (*iter)->iHp;

	m_strMonsterName = (*iter)->wstrName;

	m_iImageList.SetCurSel((*iter)->byIndex);


	UpdateData(FALSE);

	OnLbnSelchangeListImagelist();

	iter = m_listMonster.begin();
	for (int i = 0; i < iIndex; ++i)
		++iter;

	if (!((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeMonster()->empty())
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeMonster()->pop_front();
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeMonster()->push_back((*iter));

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CMonsterTool::OnLbnSelchangeListImagelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	strObject;

	iSelectIndex = m_iImageList.GetCurSel();

	if (iSelectIndex < 0)
		return;

	m_iImageList.GetText(iSelectIndex, strObject);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strObject);

	if (iter == m_mapPngImage.end())
		return;

	m_iMonsterImage.SetBitmap(*iter->second);

	int i = 0;

	for (i = 0; i < strObject.GetLength(); ++i)
	{
		//문자열을 0번 인덱스부터 검색
		//문자가 숫자인지를 검사해보자. -> 
		if (isdigit(strObject[i]) != 0)		//Tile0
		{
			break;
			//함수isdigit()의 반환값이 0이 아닌 수이면 문자가 '숫자'이다.
		}
	}
	//i -> 숫자가 있는곳까지의 Index
	strObject.Delete(0, i);
	//0번 인덱스부터 i까지 문자열을 삭제한다.
	//_tstoi() : string to int -> 문자열을 정수형으로 변경하는 함수.
	m_iObjectID = _tstoi(strObject);	//strName = Tile0
}


void CMonsterTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();
	if (!((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeMonster()->empty())
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeObject()->pop_front();
	}
}
