#pragma once

#include "MySprite.h"

enum FLYTYPE
{
	FLY = 0,
	CATCH = 1
};

class CMyAnimationSprite : public CMySprite
{
protected:
	// 생성할 Sprite 객체 수
	RECT m_rcFrame[7];

	// Sprite의 Index값
	unsigned short m_nFrame;

	// Sprite 간에 그리는 간격
	float m_interval;

	// Sprite 정보 저장소
	float m_xPos;
	float m_yPos;
	float m_moveInterval;
	float m_moveDuration;

	// 충돌 박스 정보 저장소
	RECT  m_rcCollision;

	// Sprite 를 바꾸기 위한 키값
	FLYTYPE	m_flyType;

public:
	CMyAnimationSprite();
	virtual ~CMyAnimationSprite();

	// 기본 상속
	void Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	// 충돌 시 호출
	BOOL isCollision(POINT pt);
	
	// Sprite 바꾸기
	void ChangeFlyType(FLYTYPE type);
};

