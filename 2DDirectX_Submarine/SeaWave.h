#pragma once
#include "Headers.h"

class SeaWave : public CMySprite
{
protected:
	RECT m_rcFrame[4];
	unsigned short m_nFrame;

	float m_interval;

	float m_xPos;
	float m_yPos;

	RECT  m_rcCollision;

public:
	SeaWave();
	virtual ~SeaWave();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
	
	inline RECT GetColBox() { return m_rcCollision; }
	BOOL isCollision(POINT pt);
};

