#include "StdAfx.h"
#include "EffectBridge.h"
#include "TextureMgr.h"
#include "GameObject.h"
#include "Device.h"

CEffectBridge::CEffectBridge(void)
{
}

CEffectBridge::~CEffectBridge(void)
{
	Release();
}

HRESULT	CEffectBridge::Initialize(void)
{
	return S_OK;
}
int	CEffectBridge::Update(INFO& rInfo)
{
	return 0;
}
void CEffectBridge::Render(void)
{
}
void CEffectBridge::Release(void)
{

}
