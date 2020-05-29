#pragma once
#include "Headers.h"

class SeaMonsterMgr
{
private:
	int   m_SeaMonCount;
	float m_interval;
	float m_xPos;
	float m_yPos;

private:
	class SeaMon01_Char* SeaMon_01;

public:
	SeaMonsterMgr();
	~SeaMonsterMgr();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float _time, class UserCharacter* pt_UserChar);

	inline void SeaMonOnDraw();
};
