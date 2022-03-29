#pragma once



// CMyForm 폼 뷰입니다.

#include "MyMapTool.h"
#include "ObjectTool.h"
#include "MonsterTool.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

	CFont		 m_Font;
	CMyMapTool	 m_MyMapTool;
	CObjectTool	 m_ObjectTool;
	CMonsterTool m_MonsterTool;

public:
	CMyMapTool* GetMapTool(void)
	{
		return &m_MyMapTool;
	}

	CObjectTool* GetObjectTool(void)
	{
		return &m_ObjectTool;
	}

	CMonsterTool* GetMonsterTool(void)
	{
		return &m_MonsterTool;
	}

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonMaptool();
	afx_msg void OnBnClickedButtonObject();
	afx_msg void OnBnClickedButtonObject2();
};


