#pragma once
#include "effectbridge.h"

class CEffectBuffer :
	public CEffectBridge
{
private:
	float	m_fRotateAngle;
	float	m_fTime;

private:
	void WorldMatrixCheck(INFO& rInfo);
	void FrameMove();


public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(INFO& rInfo);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CEffectBuffer(void);
	virtual ~CEffectBuffer(void);
};
