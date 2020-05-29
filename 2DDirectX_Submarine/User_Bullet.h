#pragma once
#include "Headers.h"

class User_Bullet : public CMySprite
{
protected:
	RECT m_rcFrame[15];

public:
	RECT m_rcCollision;
	unsigned short m_nFrame;

	float m_interval;
	float m_xPos;
	float m_yPos;

public:
	USER_BULLET_STATE	   m_BulletState;
	USER_UP_BULLET_STATE   m_UpBulletState;
	USER_DOWN_BULLET_STATE m_DownBulletState;

	class cMP3* pt_sound;

private:
	class Wave* pt_wave;

public:
	User_Bullet();
	virtual ~User_Bullet();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	void BulletMove();
	inline void SetBullet(unsigned short _nFrame, USER_BULLET_STATE _BulletState, float _yPos, float _xPos)
	{ m_nFrame = _nFrame, m_BulletState = _BulletState, m_yPos = _yPos, m_xPos = _xPos; }


	BOOL isCollision(RECT pt_rect);
	void ChangeBulletImg(USER_BULLET_STATE type);
};

