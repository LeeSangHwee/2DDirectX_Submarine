#pragma once
#include "Headers.h"

class UserLife : public CMySprite
{
protected:
	RECT m_rcFrame;

	float m_interval;
	float m_xPos;
	float m_yPos;
	
public:
	UserLife();
	virtual ~UserLife();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();

	inline void SetLifePos(float _yPos, float _xPos) { m_yPos = _yPos, m_xPos = _xPos; }
};

