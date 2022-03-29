
// MFC76Doc.cpp : CMFC76Doc 클래스의 구현
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


// CMFC76Doc 생성/소멸

CMFC76Doc::CMFC76Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMFC76Doc::~CMFC76Doc()
{
}

BOOL CMFC76Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMFC76Doc serialization

void CMFC76Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CMFC76Doc 진단

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


// CMFC76Doc 명령
