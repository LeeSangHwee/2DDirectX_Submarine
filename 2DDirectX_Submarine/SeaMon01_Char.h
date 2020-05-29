#pragma once
#include "Headers.h"

class SeaMon01_Char : public CMySprite
{
protected:
	RECT m_rcFrame[9];

public:
	RECT m_rcCollision;
	unsigned short m_nFrame;

	float m_CollisionInterval;
	float m_interval;
	float m_xPos;
	float m_yPos;

private:
	class UserCharacter* pt_UserChar;
	class SeaMon01_BulletMgr* pt_BulletMgr;

public:
	class cMP3* pt_sound;

public:
	SEA_MONSTER01_STATE	m_MonsterState;

public:
	SeaMon01_Char();
	virtual ~SeaMon01_Char();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);

	inline void SetptUser(UserCharacter* pt_SetUser) { pt_UserChar = pt_SetUser; }
	inline void SetSeaMon01(unsigned short _nFrame, SEA_MONSTER01_STATE	_MonsterState, float _yPos, float _xPos)
	{ m_nFrame = _nFrame, m_MonsterState = _MonsterState, m_yPos = _yPos, m_xPos = _xPos; }

	BOOL isCollision(RECT pt_rect);
	void BulletCollision(float time);
};
