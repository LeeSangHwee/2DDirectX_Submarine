#pragma once
#include "Headers.h"
#include <vector>

using namespace std;

class UserCharacter : public CMySprite
{
public:
	RECT m_rcCollision;
	int MyLife;

protected:
	RECT m_rcFrame[10];
	unsigned short m_nFrame;

	float m_interval;
	float m_xPos;
	float m_yPos;

	int MyLifeSetCount;
	int m_RespawnTime;

	int Key_SpaceReturnValue;
	int Key_ControlReturnValue;

public:
	USER_STATE	m_UserState;
	class UserBulletMgr* BulletMgr;
	class ScoreManager* ScoreMgr;

private:
	class UserLife* pt_UserLife;
	vector <UserLife> vUserLife;
	vector <UserLife>::iterator UserLife_iter;

public:
	UserCharacter();
	virtual ~UserCharacter();


	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height);
	void OnDraw();
	void OnUpdate(float time);
	void InputKey();

	inline float GetPosY() { return m_yPos; }

	BOOL isCollision(RECT pt_rect);
	void ChangeUserImg(USER_STATE type);
};

