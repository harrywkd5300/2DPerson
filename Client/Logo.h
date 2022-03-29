#pragma once
#include "scene.h"

class CLogo :
	public CScene
{
private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;

	int		m_iLoginCount;

public:
	CLogo(void);
	virtual ~CLogo(void);

public:
	virtual HRESULT	Initialize();
	virtual int		Update();
	virtual void	Render();
	virtual void	Release();

public:
	static unsigned int __stdcall ImageLoadThreadFunc(void* pArg);
	CRITICAL_SECTION  GetCrt() { return m_Crt; }
};
