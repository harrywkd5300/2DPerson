#pragma once

#include "Include.h"

class CMouse abstract
{
public:
	static D3DXVECTOR3 GetMousePos()
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hwnd, &ptMouse);

		return D3DXVECTOR3(float(ptMouse.x), 
						   float(ptMouse.y), 
						   0.f);
	}
};
