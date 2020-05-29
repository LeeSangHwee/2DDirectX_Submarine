#pragma once
#include "Headers.h"

class LoadingDisplay : public CMySprite
{
protected:
	RECT m_rcFrame[6];
	unsigned short m_nFrame;
	float m_interval;

	float m_xPos;
	float m_yPos;

public:
	int LoadCount;

public:
	LoadingDisplay();
	virtual ~LoadingDisplay();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
};

