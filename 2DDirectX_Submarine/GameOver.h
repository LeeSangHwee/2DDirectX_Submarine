#pragma once
#include "Headers.h"

class GameOver : public CMySprite
{
protected:
	RECT m_rcFrame;	
	float m_xPos;
	float m_yPos;

public:
	class cMP3* pt_sound;

public:
	GameOver();
	virtual ~GameOver();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
};

