#pragma once
#include "Headers.h"

class MenuBackGround : public CMySprite
{
protected:
	float m_xPos;
	float m_yPos;
	RECT  m_rcDraw;

public:
	MenuBackGround();
	virtual ~MenuBackGround();


	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
};

