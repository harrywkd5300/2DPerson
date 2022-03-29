#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice);

CDevice::CDevice(void)
:m_pD3D(NULL), m_pD3dDevice(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::Init3D()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 DeviceCaps;	
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));
	if(FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERROR_MSG(L"장치 생성 실패!");
		return E_FAIL;
	}
	DWORD vp;
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;		
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SerParameters(d3dpp);

	if(FAILED( m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, g_hwnd
		, vp
		, &d3dpp
		, &m_pD3dDevice) ) )
	{
		ERROR_MSG(L"디바이스 생성 실패!");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pD3dDevice, &m_pSprite)) )
	{
		ERROR_MSG(L"스프라이트객체 생성 실패!");
		return E_FAIL;
	}

	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(FontInfo));


	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_BOLD;
	FontInfo.CharSet = HANGEUL_CHARSET;

	lstrcpy(FontInfo.FaceName, L"바탕체");


	if(FAILED( D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		ERROR_MSG(L"폰트 객체 생성 실패!");
		return E_FAIL;
	}


	D3DXFONT_DESC FontCityInfo;
	ZeroMemory(&FontCityInfo, sizeof(FontCityInfo));


	FontCityInfo.Height = 10;
	FontCityInfo.Width = 7;
	FontCityInfo.Weight = FW_BOLD;
	FontCityInfo.CharSet = HANGEUL_CHARSET;

	lstrcpy(FontCityInfo.FaceName, L"휴먼옛체");


	if (FAILED(D3DXCreateFontIndirect(m_pD3dDevice, &FontCityInfo, &m_pCityFont)))
	{
		ERROR_MSG(L"폰트 객체 생성 실패!");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateLine(m_pD3dDevice, &m_pLine)))
	{
		ERROR_MSG(L"라인 객체 생성 실패!");
		return E_FAIL;
	}

	m_pLine->SetWidth(2);


	return S_OK;
}

void CDevice::SerParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;
	d3dp.BackBufferHeight = WINCY;		
	d3dp.BackBufferCount  = 1;				
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;		


	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dp.MultiSampleQuality = 0;

	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dp.hDeviceWindow = g_hwnd;		
	d3dp.Windowed	   = FALSE;			


	d3dp.EnableAutoDepthStencil = TRUE;

	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	m_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	
						D3DCOLOR_ARGB(255, 0, 0, 255), 1.f,	0);
	m_pD3dDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CDevice::Render_End(HWND& _hWnd)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, _hWnd, NULL);
}

void CDevice::Release(void)
{
	m_pSprite->Release();
	unsigned long  dwRefCnt = m_pD3dDevice->Release();
	m_pD3D->Release();
}