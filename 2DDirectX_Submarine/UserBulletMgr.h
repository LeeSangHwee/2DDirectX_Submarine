#pragma once
#include "Headers.h"

class UserBulletMgr
{
public:
	int m_BulletCount;

private:
	float m_interval;

private:
	USER_BULLET_STATE mgr_BulletState;
	class UserCharacter* UserChar;

public:
	class User_Bullet* UserBullet;

public:
	UserBulletMgr();
	~UserBulletMgr();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float _time, float UserPosX, float UserPosY);

	inline void SetBulletState(USER_BULLET_STATE _BulletState)
	{ mgr_BulletState = _BulletState; }

	void UserBulletOnDraw();
};

