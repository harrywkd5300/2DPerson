#pragma once

// CMyMapTool 대화 상자입니다.

#include "Include.h"
#include "afxwin.h"

class CMyMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMyMapTool)

private:
	map<CString, CImage*>		m_mapPngImage;
	wstring						m_wstrPath;

public:
	int		m_iDrawID;
	int		m_iBackDrawID;

public:
	void HorizontalScroll(void);

public:
	CMyMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMyMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_MapListBox;
	CStatic m_Picture;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeListMakemap();
	CString m_strPath;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	CComboBox m_ComboBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	BOOL m_Tilemap;
	BOOL m_Font;
	afx_msg void OnBnClickedCheckTilemap();
	afx_msg void OnBnClickedCheckFont();
};
