// MonsterTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC76.h"
#include "MonsterTool.h"
#include "afxdialogex.h"

#include "TextureMgr.h"
#include "MainFrm.h"
#include "MFC76View.h"
#include "BackGround.h"


// CMonsterTool ��ȭ �����Դϴ�.

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


// CMonsterTool �޽��� ó�����Դϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR szFullPath[MAX_PATH] = L"";			//��ӵ� ������ ��θ� ������ ����.
	
	TCHAR szFileName[MIN_STR] = L"";
	
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
		m_iImageList.AddString(strImageName);
	}
	UpdateData(FALSE);
}


void CMonsterTool::OnBnClickedButtonAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (FilePath == _T(""))
	{
		MessageBox(L"�������!!", L"���忡��!");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	wstring FilePath = m_strPathName.operator LPCWSTR();
	UpdateData(FALSE);
	if (m_strPathName == _T(""))
	{
		MessageBox(L"�ε����!!", L"�ε忡��!");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		//���ڿ��� 0�� �ε������� �˻�
		//���ڰ� ���������� �˻��غ���. -> 
		if (isdigit(strObject[i]) != 0)		//Tile0
		{
			break;
			//�Լ�isdigit()�� ��ȯ���� 0�� �ƴ� ���̸� ���ڰ� '����'�̴�.
		}
	}
	//i -> ���ڰ� �ִ°������� Index
	strObject.Delete(0, i);
	//0�� �ε������� i���� ���ڿ��� �����Ѵ�.
	//_tstoi() : string to int -> ���ڿ��� ���������� �����ϴ� �Լ�.
	m_iObjectID = _tstoi(strObject);	//strName = Tile0
}


void CMonsterTool::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnClose();
	if (!((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeMonster()->empty())
	{
		((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetMakeObject()->pop_front();
	}
}
