#pragma once
#include "Headers.h"

enum START_BUTTON_IMG
{
	COL_START_OFF = 0,
	COL_START_ON = 1
};

class StartButton : public CMySprite
{
protected:
	RECT m_ButtonImg[2];
	RECT  m_rcCollision;

	START_BUTTON_IMG m_eChageButton;

	float PosX;
	float PosY;

public:
	StartButton();
	virtual ~StartButton();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	BOOL isCollision(POINT pt);
	void ChangeButtonImg(START_BUTTON_IMG type);
};

