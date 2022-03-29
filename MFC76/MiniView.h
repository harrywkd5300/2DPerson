#pragma once


// CMiniView 뷰입니다.

class CMFC76View;
class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

private:
	CMFC76View*			m_MainView;

protected:
	CMiniView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMiniView();

public:
	void SetMinView(CMFC76View* mainView) {  m_MainView = mainView; }

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
};


