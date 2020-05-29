#pragma once
#include "MySprite.h"

class CSubMarineSprite : public CMySprite
{
protected:
	float m_xPos;
	float m_yPos;
	RECT  m_rcDraw;

public:
	CSubMarineSprite();
	virtual ~CSubMarineSprite();


	void Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
};

