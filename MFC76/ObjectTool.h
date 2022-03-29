#pragma once

#include "Include.h"
#include "afxwin.h"


// CObjectTool ��ȭ �����Դϴ�.

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

private:
	wstring						m_wstrPath;
	CString						strName;
	CString						strPath;
	bool						bStartRadioButtonCheck;



public:
	map<CString, CImage*>		m_mapPngImage;
	list<IMAGE_PATH*>			m_listImagePath;
	int			m_iObjectID;
	INFO*		pObject;
	OBJECT*		pCityObject;
	bool		bObjectRadio;
	bool		bCityObjectRadio;

public:
	void HorizontalScroll();
	CString& GetPathID() {  return strName;  }

public:
	CObjectTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjectTool();


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	CListBox m_ObjectCreateList;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeListCreate();
	CStatic m_ObjectImage;
	afx_msg void OnClose();
	float m_fPostionX;
	float m_fPostionY;
	float m_fPostionZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	CListBox m_listBoxAdd;
	afx_msg void OnBnClickedButtonYsort();
	CString m_FileName;
	afx_msg void OnEnChangeEditFilename();
	CButton m_iRadio[2];
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
};
