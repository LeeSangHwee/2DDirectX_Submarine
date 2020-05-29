#pragma once
#include "Headers.h"

class InGameBackGround : public CMySprite
{
protected:
	float m_xPos;
	float m_yPos;
	RECT  m_rcDraw;

public:
	InGameBackGround();
	virtual ~InGameBackGround();


	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
};

