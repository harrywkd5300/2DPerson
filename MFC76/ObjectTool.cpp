// ObjectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC76.h"
#include "ObjectTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MFC76View.h"
#include "MiniView.h"
#include "BackGround.h"

// CObjectTool ��ȭ �����Դϴ�.

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
		iter->second->Destroy();		//�̹��� ��ü�� �ı�����.
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


// CObjectTool �޽��� ó�����Դϴ�.

void CObjectTool::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	//if (((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetBackObject()->empty() || m_listBoxAdd.GetCount() <= 0)
	//{
	//	MessageBox(L"������ �����Ͱ� �����ϴ�!!", L"������ ����!!");
	//	return;
	//}

	//IXMLDOMDocument2Ptr pDoc;

	//MSXML2::IXMLDOMProcessingInstructionPtr pPl;

	//pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	//MSXML2::IXMLDOMElementPtr	pRootElementObject;

	////���� ������ Element����.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (FilePath == _T(""))
	{
		MessageBox(L"�������!!",L"���忡��!");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	wstring FilePath = m_FileName.operator LPCWSTR();
	UpdateData(FALSE);

	if (m_FileName == _T(""))
	{
		MessageBox(L"�ε����!!", L"�ε忡��!");
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
		m_ObjectCreateList.AddString(strImageName);
	}
	HorizontalScroll();
	CDialog::OnDropFiles(hDropInfo);
	UpdateData(FALSE);
}


void CObjectTool::OnLbnSelchangeListCreate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


void CObjectTool::OnBnClickedRadio1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!bObjectRadio)
	{
		bObjectRadio = true;
		bCityObjectRadio = false;
	}
}


void CObjectTool::OnBnClickedRadio3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!bCityObjectRadio)
	{
		bCityObjectRadio = true;
		bObjectRadio = false;
	}
}
