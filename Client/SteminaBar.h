#pragma once
#include "GameObject.h"
class CSteminaBar :
	public CGameObject
{
	float   m_fScaleXSize;
public:
	CSteminaBar();
	virtual ~CSteminaBar();

public:
	void  SetScaleSizeX(float size) { m_fScaleXSize = size; }
	float GetScaleSizeX() { return m_fScaleXSize; }

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);
};

