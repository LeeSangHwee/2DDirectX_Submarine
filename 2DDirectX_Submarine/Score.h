#pragma once
#include "Headers.h"

class Score : public CMySprite
{
protected:
	RECT m_rcFrame[11];
	unsigned short m_nFrame;
	float m_xPos;
	float m_yPos;

public:
	Score();
	virtual ~Score();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();

	inline void SetScore(unsigned short _nFrame, float _yPos, float _xPos)
	{ m_nFrame = _nFrame, m_yPos = _yPos, m_xPos = _xPos; }
};

