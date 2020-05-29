#pragma once
#include "Headers.h"

enum EXIT_BUTTON_IMG
{
	COL_EXIT_OFF = 0,
	COL_EXIT_ON = 1
};

class ExitButton : public CMySprite
{
protected:
	RECT m_ButtonImg[2];
	RECT  m_rcCollision;

	EXIT_BUTTON_IMG m_eChageButton;

	float PosX;
	float PosY;

public:
	ExitButton();
	virtual ~ExitButton();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	BOOL isCollision(POINT pt);
	void ChangeButtonImg(EXIT_BUTTON_IMG type);
};

