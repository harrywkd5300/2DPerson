#pragma once

#include "Include.h"

// 클래스 헤더를 선언하는 방식
// 0.상수(enum, const) 
// 1.생성자들
// 2.소멸자
// get함수
// set함수
// 3.public 변수
// 4.public 함수
// 5.protected 변수
// 6.protected 함수
// 7.private 변수
// 8.private 함수
// 9.이 클래스를 객체화하기 위한 함수
// 10.객체를 메모리 해제하기위한 함수

class CTimer
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	float Get_TimeDelta() const
	{
		return m_fTimeDelta;
	}

public:
	HRESULT Initialize();
	void	Computer_Timer();


private:
	LARGE_INTEGER 			m_FrameTime;
	LARGE_INTEGER 			m_FixTime;
	LARGE_INTEGER 			m_LastTime;
	LARGE_INTEGER 			m_CpuTick;
private:
	float		m_fTimeDelta;


public:
	static CTimer* Create(void);   // 생성과 관련된 함수!!

public:
	void Release();
};

