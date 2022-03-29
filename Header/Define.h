#pragma once

#define WINCX 800
#define WINCY 600

const WORD MIN_STR	  = 64;
const WORD MIDDLE_STR = 128;
const WORD MAX_STR	  = 256;

const int TILEX = 40;
const int TILEY = 60;

const int TILECX = 64;
const int TILECY = 32;

#define CENTER_POS(x) (x / 2.f)

#define KEYPRESS(DST, SRC) (DST & SRC)

#define DECLARE_SINGLETON(ClassName)	\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;				\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}									\
private:								\
	static ClassName* m_pInstance;


#define IMPLEMENT_SINGLETON(ClassName)	\
	ClassName* ClassName::m_pInstance = NULL;

#define ERROR_MSG(Text)		\
	MessageBox(g_hwnd, Text, L"Error!", MB_OK)


#define GET_TIME CTimeMgr::GetInstance()->GetTime()