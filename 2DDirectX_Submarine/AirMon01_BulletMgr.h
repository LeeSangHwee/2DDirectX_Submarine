#pragma once
#include "Headers.h"

class AirMon01_BulletMgr
{
public:
	int m_BulletCount;

private:
	float m_interval;

private:
	class AirMon01_Bullet* pt_MonBullet;

public:
	AirMon01_BulletMgr();
	~AirMon01_BulletMgr();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float _time, float MonPosX, float MonPosY, AIR_MONSTER01_STATE _MosnterState, class UserCharacter* _UserChar);
};

