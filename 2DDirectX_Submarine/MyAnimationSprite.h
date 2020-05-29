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
	// ������ Sprite ��ü ��
	RECT m_rcFrame[7];

	// Sprite�� Index��
	unsigned short m_nFrame;

	// Sprite ���� �׸��� ����
	float m_interval;

	// Sprite ���� �����
	float m_xPos;
	float m_yPos;
	float m_moveInterval;
	float m_moveDuration;

	// �浹 �ڽ� ���� �����
	RECT  m_rcCollision;

	// Sprite �� �ٲٱ� ���� Ű��
	FLYTYPE	m_flyType;

public:
	CMyAnimationSprite();
	virtual ~CMyAnimationSprite();

	// �⺻ ���
	void Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	// �浹 �� ȣ��
	BOOL isCollision(POINT pt);
	
	// Sprite �ٲٱ�
	void ChangeFlyType(FLYTYPE type);
};

