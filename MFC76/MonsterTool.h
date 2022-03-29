#pragma once

#include "Include.h"
#include "afxwin.h"
// CMonsterTool 대화 상자입니다.

class CMonsterTool : public CDialog
{
	DECLARE_DYNAMIC(CMonsterTool)

private:
	CString		m_strSavePath;
	UNIT_DATA*	pObject;
	CString		strObject;
	int			m_iObjectID;
	int			iSelectIndex;

public:
	list<IMAGE_PATH*>		m_listImagePath;
	map<CString, CImage*>	m_mapPngImage;
	list<UNIT_DATA*>		m_listMonster;

public:
	void HorizontalImageScroll();
	void HorizontalAddScroll();

public:
	CMonsterTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonsterTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMonsterName;
	int m_iAtk;
	int m_iDef;
	int m_iLevel;
	int m_iHp;
	CListBox m_iImageList;
	CListBox m_iAddList;
	CStatic m_iMonsterImage;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnLbnSelchangeListAddlist();
	afx_msg void OnLbnSelchangeListImagelist();
	CString m_strPathName;
	afx_msg void OnClose();
};
