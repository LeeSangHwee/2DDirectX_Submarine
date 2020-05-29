#pragma once
#include "Headers.h"

class SeaMon01_Bullet : public CMySprite
{
protected:
	RECT m_rcFrame[7];

public:
	RECT m_rcCollision;
	unsigned short m_nFrame;
	float m_CollisionInterval;
	float m_interval;

	float m_xPos;
	float m_yPos;

private:
	class UserCharacter* pt_UserChar;

public:
	class cMP3* pt_sound;

public:
	SEAMON01_BULLET_STATE m_BulletState;

public:
	SeaMon01_Bullet();
	virtual ~SeaMon01_Bullet();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	void BulletMove();
	inline void SetptUser(UserCharacter* pt_SetUser) { pt_UserChar = pt_SetUser; }
	inline void SetBullet(unsigned short _nFrame, SEAMON01_BULLET_STATE _BulletState, float _yPos, float _xPos)
	{ m_nFrame = _nFrame, m_BulletState = _BulletState, m_yPos = _yPos, m_xPos = _xPos; }

	BOOL isCollision(RECT pt_rect);
	void BulletCollision(float time);
};
