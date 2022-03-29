#pragma once

#include "Include.h"

// Ŭ���� ����� �����ϴ� ���
// 0.���(enum, const) 
// 1.�����ڵ�
// 2.�Ҹ���
// get�Լ�
// set�Լ�
// 3.public ����
// 4.public �Լ�
// 5.protected ����
// 6.protected �Լ�
// 7.private ����
// 8.private �Լ�
// 9.�� Ŭ������ ��üȭ�ϱ� ���� �Լ�
// 10.��ü�� �޸� �����ϱ����� �Լ�

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
	static CTimer* Create(void);   // ������ ���õ� �Լ�!!

public:
	void Release();
};

