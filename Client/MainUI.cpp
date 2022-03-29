#include "stdafx.h"
#include "MainUI.h"
#include "Device.h"
#include "TextureMgr.h"
#include "SceneMgr.h"

CMainUI::CMainUI()
{
}

CMainUI::~CMainUI()
{
}

HRESULT	CMainUI::Initialize(void)
{
	m_tUnit.vPos = D3DXVECTOR3(400.f, 555.f, 0.f);
	lstrcpy(m_tUnit.wstrObjKey, L"UI");
	lstrcpy(m_tUnit.wstrStateKey, L"Pannel");

	if (CSceneMgr::GetInstance()->GetScene() != SCENE_BATTLE)
		m_tUnit.byDrawID = 0;
	else
		m_tUnit.byDrawID = 1;


	m_eRenderID = RENDER_TYPE_UI;
	return S_OK;
}

int	CMainUI::Update(void)
{
	return 0;
}

void	CMainUI::Render(void)
{
	if (CSceneMgr::GetInstance()->GetScene() == SCENE_BATTLE)
	{
		const TEX_INFO* pMainUITex =
			CTextureMgr::GetInstance()->GetTexture(L"UI",
				L"Pannel",
				5);
		if (pMainUITex == NULL)
		{
			ERROR_MSG(L"MainUI 이미지 X");
			return;
		}

		D3DXMatrixTranslation(&matTrans
			, 98.f
			, 555.f
			, 0.f);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

		m_tUnit.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

		float fX = CENTER_POS(pMainUITex->ImageInfo.Width);
		float fY = CENTER_POS(pMainUITex->ImageInfo.Height);

		CDevice::GetInstance()->GetSprite()->Draw(
			pMainUITex->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	const TEX_INFO* pMainUITex =
		CTextureMgr::GetInstance()->GetTexture(m_tUnit.wstrObjKey,
											   m_tUnit.wstrStateKey,
											   m_tUnit.byDrawID);
	if (pMainUITex == NULL)
	{
		ERROR_MSG(L"MainUI 이미지 X");
		return;
	}

	D3DXMatrixTranslation(&matTrans
		, m_tUnit.vPos.x
		, m_tUnit.vPos.y
		, 0.f);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tUnit.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tUnit.matWorld);

	float fX = CENTER_POS(pMainUITex->ImageInfo.Width);
	float fY = CENTER_POS(pMainUITex->ImageInfo.Height);

	CDevice::GetInstance()->GetSprite()->Draw(
		pMainUITex->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void	CMainUI::Release(void)
{

}
