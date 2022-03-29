#pragma once


// CMiniView ���Դϴ�.

class CMFC76View;
class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

private:
	CMFC76View*			m_MainView;

protected:
	CMiniView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMiniView();

public:
	void SetMinView(CMFC76View* mainView) {  m_MainView = mainView; }

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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


