
// MFC76Doc.cpp : CMFC76Doc Ŭ������ ����
//

#include "stdafx.h"
#include "MFC76.h"

#include "MFC76Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC76Doc

IMPLEMENT_DYNCREATE(CMFC76Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFC76Doc, CDocument)
END_MESSAGE_MAP()


// CMFC76Doc ����/�Ҹ�

CMFC76Doc::CMFC76Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMFC76Doc::~CMFC76Doc()
{
}

BOOL CMFC76Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CMFC76Doc serialization

void CMFC76Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CMFC76Doc ����

#ifdef _DEBUG
void CMFC76Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC76Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC76Doc ���
