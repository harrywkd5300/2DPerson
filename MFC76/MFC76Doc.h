
// MFC76Doc.h : CMFC76Doc Ŭ������ �������̽�
//


#pragma once


class CMFC76Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC76Doc();
	DECLARE_DYNCREATE(CMFC76Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMFC76Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


