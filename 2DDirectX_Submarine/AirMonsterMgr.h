#pragma once
#include "Headers.h"

class AirMonsterMgr
{
private:
	int   m_AirMonCount;
	float m_interval;
	float m_xPos;
	float m_yPos;

private:
	class AirMon01_Char* AirMon_01;

public:
	AirMonsterMgr();
	~AirMonsterMgr();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float _time, class UserCharacter* pt_UserChar);

	inline void AirMonOnDraw();
};
