// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC76.h"
#include "ObjectTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC76View.h"
#include "MiniView.h"
#include "BackGround.h"

// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent), strName(L""), pObject(NULL), strPath(L"")
	, m_fPostionX(0)
	, m_fPostionY(0)
	, m_fPostionZ(0)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_fScaleX(1)
	, m_fScaleY(1)
	, m_fScaleZ(1)
	, m_FileName(_T(""))
	, bStartRadioButtonCheck(FALSE)
	, pCityObject(NULL)
	, bObjectRadio(false)
	, bCityObjectRadio(false)
{

}

CObjectTool::~CObjectTool()
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

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CREATE, m_ObjectCreateList);
	DDX_Control(pDX, IDC_STATIC_OBJECT, m_ObjectImage);
	DDX_Text(pDX, IDC_EDIT_POSIONX, m_fPostionX);
	DDX_Text(pDX, IDC_EDIT_POSIONY, m_fPostionY);
	DDX_Text(pDX, IDC_EDIT_POSTIONZ, m_fPostionZ);
	DDX_Text(pDX, IDC_EDIT_ROTATIONX, m_fRotationX);
	DDX_Text(pDX, IDC_EDIT_ROTATIONY, m_fRotationY);
	DDX_Text(pDX, IDC_EDIT_ROTATIONZ, m_fRotationZ);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_fScaleZ);
	DDX_Control(pDX, IDC_LIST_ADD, m_listBoxAdd);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Control(pDX, IDC_RADIO1, m_iRadio[0]);
	DDX_Control(pDX, IDC_RADIO3, m_iRadio[1]);
	if (bStartRadioButtonCheck)
	{
		bStartRadioButtonCheck = false;
		m_iRadio[0].SetCheck(TRUE);
	}
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CObjectTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CObjectTool::OnBnClickedButtonLoad)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST_CREATE, &CObjectTool::OnLbnSelchangeListCreate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_YSORT, &CObjectTool::OnBnClickedButtonYsort)
	ON_EN_CHANGE(IDC_EDIT_FILENAME, &CObjectTool::OnEnChangeEditFilename)
	ON_BN_CLICKED(IDC_RADIO1, &CObjectTool::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CObjectTool::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.

void CObjectTool::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	//if (((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject()->empty() || m_listBoxAdd.GetCount() <= 0)
	//{
	//	MessageBox(L"저장할 데이터가 없습니다!!", L"데이터 에러!!");
	//	return;
	//}

	//IXMLDOMDocument2Ptr pDoc;

	//MSXML2::IXMLDOMProcessingInstructionPtr pPl;

	//pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	//MSXML2::IXMLDOMElementPtr	pRootElementObject;

	////실제 데이터 Element선언.
	//MSXML2::IXMLDOMElementPtr	pElement_vPos;
	//MSXML2::IXMLDOMElementPtr	pElement_vDir;
	//MSXML2::IXMLDOMElementPtr	pElement_vLook;
	//MSXML2::IXMLDOMElementPtr	pElement_Att;

	//MSXML2::IXMLDOMElementPtr	pElement_vSize;
	//MSXML2::IXMLDOMElementPtr	pElement_Regen;
	//MSXML2::IXMLDOMElementPtr	pElement_Price;
	//MSXML2::IXMLDOMElementPtr	pElement_Type;

	//MSXML2::IXMLDOMElementPtr	pElement_Line;




	wstring FilePath = m_FileName.operator LPCWSTR();
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (FilePath == _T(""))
	{
		MessageBox(L"저장실패!!",L"저장에러!");
		return;
	}
	else
	{
		if (bObjectRadio)
		{
			FilePath = L"../Data/" + FilePath;

			vector<INFO*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject();
			DWORD dwByte = 0;
			HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_WRITE, NULL, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			for (size_t i = 0; i < pVector->size(); ++i)
			{
				WriteFile(hFile, (*pVector)[i], sizeof(INFO), &dwByte, NULL);
			}
			CloseHandle(hFile);
		}
		else if (bCityObjectRadio)
		{
			FilePath = L"../Data/" + FilePath;

			vector<OBJECT*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackCity();
			DWORD dwByte = 0;
			HANDLE hFile = CreateFile(FilePath.c_str(), GENERIC_WRITE, NULL, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			for (size_t i = 0; i < pVector->size(); ++i)
			{
				WriteFile(hFile, (*pVector)[i], sizeof(OBJECT), &dwByte, NULL);
			}
			CloseHandle(hFile);
		}
	}
}


void CObjectTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	wstring FilePath = m_FileName.operator LPCWSTR();
	UpdateData(FALSE);

	if (m_FileName == _T(""))
	{
		MessageBox(L"로드실패!!", L"로드에러!");
		return;
	}
	else
	{
		if (bObjectRadio)
		{
			FilePath = L"../Data/" + FilePath;
			vector<INFO*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject();
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
				INFO* pObjectData = new INFO;
				ReadFile(hFile, pObjectData, sizeof(INFO), &dwByte, NULL);
				if (dwByte == 0)
				{
					safeDelete(pObjectData);
					break;
				}
				pVector->push_back(pObjectData);
			}
			CloseHandle(hFile);
		}
		else if (bCityObjectRadio)
		{
			FilePath = L"../Data/" + FilePath;
			vector<OBJECT*>* pVector = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackCity();
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
				OBJECT* pObjectData = new OBJECT;
				ReadFile(hFile, pObjectData, sizeof(OBJECT), &dwByte, NULL);
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
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->GetMinView()->Invalidate(FALSE);
}
void CObjectTool::HorizontalScroll(void)
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_ObjectCreateList.GetDC();
	for (int i = 0; i < m_ObjectCreateList.GetCount(); ++i)
	{
		m_ObjectCreateList.GetText(i, strName);
		Size = pDc->GetTextExtent(strName);
		if (Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}
	m_ObjectCreateList.ReleaseDC(pDc);

	if (m_ObjectCreateList.GetHorizontalExtent() < iCX)
	{
		m_ObjectCreateList.SetHorizontalExtent(iCX);
	}

}

void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	CStatic* staticsize1 = (CStatic*)GetDlgItem(IDC_STATIC_OBJECT);
	staticsize1->MoveWindow(410, 50, 200, 200);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";

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
		m_ObjectCreateList.AddString(strImageName);
	}
	HorizontalScroll();
	CDialog::OnDropFiles(hDropInfo);
	UpdateData(FALSE);
}


void CObjectTool::OnLbnSelchangeListCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	strName;

	int iSelectIndex = m_ObjectCreateList.GetCurSel();

	if (iSelectIndex < 0)
		return;

	m_ObjectCreateList.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strName);

	if (iter == m_mapPngImage.end())
		return;

	m_ObjectImage.SetBitmap(*iter->second);

	strPath = strName;

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
	m_iObjectID = _tstoi(strName);	//strName = Tile0

	if (bObjectRadio)
	{
		if (pObject != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeObject()->pop_front();
			safeDelete(pObject);
		}

		pObject = new INFO;
		ZeroMemory(pObject, sizeof(INFO));

		pObject->byDrawID = m_iObjectID;
		lstrcpy(pObject->strName, strPath);


		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeObject()->push_back(pObject);
	}
	else if (bCityObjectRadio)
	{
		if (pCityObject != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeCity()->pop_front();
			safeDelete(pCityObject);
		}

		list<IMAGE_PATH*>::iterator iter_image = m_listImagePath.begin();
		int iIndex2 = m_ObjectCreateList.GetCurSel();

		for (int i = 0; i < iIndex2; ++i)
			++iter_image;
		pCityObject = new OBJECT;
		ZeroMemory(pCityObject, sizeof(OBJECT));
		pCityObject->byDrawID = m_iObjectID;

		lstrcpy(pCityObject->wstrImagePath, (*iter_image)->wstrPath.c_str());
		lstrcpy(pCityObject->wstrObjKey, (*iter_image)->wstrObjKey.c_str());
		lstrcpy(pCityObject->wstrStateKey, (*iter_image)->wstrStateKey.c_str());


		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeCity()->push_back(pCityObject);
	}

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}


void CObjectTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();
	if (pObject != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeObject()->pop_front();
		safeDelete(pObject);
	}
	if (pCityObject != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeCity()->pop_front();
		safeDelete(pCityObject);
	}
}

void CObjectTool::OnBnClickedButtonYsort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->YSorting();

	m_listBoxAdd.ResetContent();

	vector<INFO*>::iterator iter_begin = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject()->begin();
	vector<INFO*>::iterator iter_end   = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject()->end();


	for (size_t i = 0; i < ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject()->size(); ++i)
	{
		wstring iAdd = (*iter_begin)->strName;
		m_listBoxAdd.AddString(iAdd.c_str());
		++iter_begin;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}

void CObjectTool::OnEnChangeEditFilename()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CObjectTool::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bObjectRadio)
	{
		bObjectRadio = true;
		bCityObjectRadio = false;
	}
}


void CObjectTool::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!bCityObjectRadio)
	{
		bCityObjectRadio = true;
		bObjectRadio = false;
	}
}
