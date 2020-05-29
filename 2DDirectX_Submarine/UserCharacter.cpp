#include "UserCharacter.h"
#include "UserBulletMgr.h"
#include "UserLife.h"

#include "SeaMonsterMgr.h"
#include "AirMonsterMgr.h"
#include "SeaMon01_BulletMgr.h"
#include "AirMon01_BulletMgr.h"
#include "ScoreManager.h"

UserCharacter::UserCharacter()
{
	srand(time(NULL));

	// User Initialize
	m_nFrame = 0;
	m_interval = 0.0f;	

	m_xPos = 50.0f;
	m_yPos = 300.0f;
	
	MyLife = 3;
	m_RespawnTime = 0;

	m_UserState = USER_STATE::USER_IDLE;

	// Create Bullet Manager
	BulletMgr = new UserBulletMgr;
	
	// Create Score Manager
	ScoreMgr = new ScoreManager;

	// Create My Life
	pt_UserLife = new UserLife;

	for (int count = MyLife; count > 0; count--)
		vUserLife.push_back(*pt_UserLife);
}

UserCharacter::~UserCharacter()
{

}

void UserCharacter::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);
	MyLifeSetCount = 0;
	
	// Move
	SetRect(&m_rcFrame[0], 0, 0, 120, 67);
	SetRect(&m_rcFrame[1], 0, 67, 120, 67);
	SetRect(&m_rcFrame[2], 0, 134, 120, 67);

	// Hit
	SetRect(&m_rcFrame[3], 0, 201, 120, 67);

	// User Explosion
	SetRect(&m_rcFrame[4], 120, 0, 165, 160);
	SetRect(&m_rcFrame[5], 285, 0, 165, 160);
	SetRect(&m_rcFrame[6], 450, 0, 165, 160);
	SetRect(&m_rcFrame[7], 120, 160, 165, 160);
	SetRect(&m_rcFrame[8], 285, 160, 165, 160);
	SetRect(&m_rcFrame[9], 450, 160, 165, 160);

	// Bullet bmp Create
	BulletMgr->Create(pDevice);

	// Score Initialize
	ScoreMgr->Create(pDevice);

	// Life bmp Create
	for (UserLife_iter = vUserLife.begin(); UserLife_iter != vUserLife.end(); UserLife_iter++)
	{
		UserLife_iter->Create(pDevice, "Sprite/MyShipLife.bmp", 40, 20);
		UserLife_iter->SetLifePos(530.0f, 10.0f + (50 * MyLifeSetCount));
		MyLifeSetCount++;
	}		
}

void UserCharacter::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_nFrame].bottom;


	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}

void UserCharacter::OnUpdate(float time)
{
	if (m_UserState != USER_STATE::USER_HIT)
		InputKey();

	m_interval += time;
	if (m_interval > 0.1f)
	{
		m_interval = 0.0f;

		switch (m_UserState)
		{
			case USER_STATE::USER_UP:	m_nFrame = 0; break;
			case USER_STATE::USER_IDLE: m_nFrame = 1; break;
			case USER_STATE::USER_DOWN: m_nFrame = 2; break;
			case USER_STATE::USER_HIT:
			{
				if (4 <= m_nFrame && m_nFrame < 9) m_nFrame++;
				else if (m_nFrame >= 9) m_UserState = USER_STATE::USER_RESET;
				else if (m_nFrame != 4) m_nFrame = 4;
			}break;
			case USER_STATE::USER_RESET:
			{
				if (m_RespawnTime == 0)
				{
					vUserLife.pop_back();
					MyLife--;
					m_xPos = 50.0f;
					m_yPos = 300.0f;
					m_RespawnTime++;
				}
				else if (m_RespawnTime < 25)
				{
					m_RespawnTime++;
					if (m_nFrame == 3) m_nFrame = 1;
					else if (m_nFrame == 1) m_nFrame = 3;
					else if (m_nFrame != 3) m_nFrame = 3;
				}
				else if (m_RespawnTime >= 25)
				{
					m_RespawnTime = 0.0f;
					m_UserState = USER_STATE::USER_IDLE;
				}
			}break;
		}
	}

	OnDraw();

	// Bullet Update
	BulletMgr->OnUpdate(time, m_xPos, m_yPos);

	// Score Update
	ScoreMgr->OnUpdate(time);

	// Life OnDraw
	for (UserLife_iter = vUserLife.begin(); UserLife_iter != vUserLife.end(); UserLife_iter++)
		UserLife_iter->OnDraw();
}

void UserCharacter::InputKey()
{
	// Move - Y Pos
	if (125 <= m_yPos && m_yPos <= 470)
	{
		if(m_UserState < USER_STATE::USER_HIT)
			ChangeUserImg(USER_STATE::USER_IDLE);

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (m_UserState < USER_STATE::USER_HIT)
				ChangeUserImg(USER_STATE::USER_UP);
			m_yPos -= 0.08f;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			if (m_UserState < USER_STATE::USER_HIT)
				ChangeUserImg(USER_STATE::USER_DOWN);
			m_yPos += 0.08f;
		}
	}

	if (125 > m_yPos)
	{
		if (m_UserState < USER_STATE::USER_HIT)
			ChangeUserImg(USER_STATE::USER_IDLE);
		m_yPos = 125.01f;
	}
	else if (m_yPos > 470)
	{
		if (m_UserState < USER_STATE::USER_HIT)
			ChangeUserImg(USER_STATE::USER_IDLE);
		m_yPos = 469.99f;
	}

	// Move - X Pos
	if (0 <= m_xPos && m_xPos <= 690)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_xPos += 0.08;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_xPos -= 0.08;
	}

	if (0 > m_xPos) m_xPos = 0.05f;
	else if (m_xPos > 690) m_xPos = 689.99f;

	//Create Bullet
	if (BulletMgr->m_BulletCount > 0)
	{
		if (GetAsyncKeyState(VK_SPACE) & Key_SpaceReturnValue)
			BulletMgr->SetBulletState(USER_BULLET_STATE::BULLET_MIDDLE);

		Key_SpaceReturnValue = 0x0001;
	}
	else Key_SpaceReturnValue = 0x8000;

	if (BulletMgr->m_BulletCount > 1)
	{
		if (GetAsyncKeyState(VK_CONTROL) & Key_ControlReturnValue)
			BulletMgr->SetBulletState(USER_BULLET_STATE::BULLET_UP);

		Key_ControlReturnValue = 0x0001;
	}
	else Key_ControlReturnValue = 0x8000;
}

BOOL UserCharacter::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void UserCharacter::ChangeUserImg(USER_STATE type)
{
	if (m_UserState == type)
		return;

	if(m_RespawnTime == 0)
		m_UserState = type;
}