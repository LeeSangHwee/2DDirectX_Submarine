#pragma once
#include "Headers.h"

class SeaMon01_BulletMgr
{
public:
	int m_BulletCount;

private:
	float m_interval;

private:
	class SeaMon01_Bullet* pt_MonBullet;

public:
	SeaMon01_BulletMgr();
	~SeaMon01_BulletMgr();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float _time, float MonPosX, float MonPosY, class UserCharacter* _UserChar, SEA_MONSTER01_STATE m_MonsterState);
};

