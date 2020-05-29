#include "SeaMon01_Char.h"
#include "SeaMon01_BulletMgr.h"
#include "UserCharacter.h"
#include "UserBulletMgr.h"
#include "User_Bullet.h"
#include "ScoreManager.h"
#include "cMP3.h"

SeaMon01_Char::SeaMon01_Char()
{
	// Monster Initialize
	m_nFrame = -1;
	m_CollisionInterval = 0.0f;
	m_interval = 0.3f;

	m_xPos = 0.0f;
	m_yPos = 0.0f;
	
	m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_OFF;

	// Create Bullet Manager
	pt_BulletMgr = new SeaMon01_BulletMgr;

	// Create Sound
	pt_sound = new cMP3;
}

SeaMon01_Char::~SeaMon01_Char()
{

}

void SeaMon01_Char::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Enemy Explosion
	SetRect(&m_rcFrame[0], 150, 0, 120, 120);
	SetRect(&m_rcFrame[1], 270, 0, 120, 120);
	SetRect(&m_rcFrame[2], 390, 0, 120, 120);
	SetRect(&m_rcFrame[3], 150, 120, 120, 120);
	SetRect(&m_rcFrame[4], 270, 120, 120, 120);
	SetRect(&m_rcFrame[5], 390, 120, 120, 120);

	// Enemy Ship
	SetRect(&m_rcFrame[6], 0, 0, 150, 67);
	SetRect(&m_rcFrame[7], 0, 67, 150, 67);
	SetRect(&m_rcFrame[8], 0, 134, 150, 67);

	pt_BulletMgr->Create(pDevice);
}

void SeaMon01_Char::OnDraw()
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

void SeaMon01_Char::OnUpdate(float time)
{
	if (m_MonsterState > SEA_MONSTER01_STATE::SEA_MONSTER01_DEL)
	{
		// Get User Pos
		if (m_MonsterState != SEA_MONSTER01_STATE::SEA_MONSTER01_HIT)
		{
			if (pt_UserChar->GetPosY() < m_yPos - 1)
				m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_UP;
			else if (pt_UserChar->GetPosY() > m_yPos + 1)
				m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_DOWN;
			else m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_IDLE;

			// Collision
			BulletCollision(time);

			// Pos X Move
			m_xPos -= 0.015f;
			if (m_xPos <= -150.005f) m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_DEL;
		}

		// Pos Y Move
		m_interval += time;
		if (m_interval > 0.1)
		{
			m_interval = 0.0f;

			if (200 <= m_yPos && m_yPos <= 500)
			{
				switch (m_MonsterState)
				{
					case SEA_MONSTER01_STATE::SEA_MONSTER01_UP:
					{
						m_nFrame = 6;
						m_yPos -= 0.01f;
					}break;
					case SEA_MONSTER01_STATE::SEA_MONSTER01_IDLE:
					{
						m_nFrame = 7;
					}break;
					case SEA_MONSTER01_STATE::SEA_MONSTER01_DOWN:
					{
						m_nFrame = 8;
						m_yPos += 0.01f;
					}break;
					case SEA_MONSTER01_STATE::SEA_MONSTER01_HIT:
					{
						if (m_nFrame < 6) m_nFrame++;
						else if (m_nFrame != 0) m_nFrame = 0;
						
						if (m_nFrame == 6)
							m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_DEL;						
					}break;
				}
			}
			if (200 > m_yPos) m_yPos = 200.0f;
			else if (m_yPos > 500) m_yPos = 500.0f;
		}	
		OnDraw();
	}

	pt_BulletMgr->OnUpdate(time, m_xPos, m_yPos, pt_UserChar, m_MonsterState);
}

BOOL SeaMon01_Char::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void SeaMon01_Char::BulletCollision(float time)
{
	m_CollisionInterval += time;
	if (m_CollisionInterval > 0.2f)
	{
		m_CollisionInterval = 0.0f;

		for (int i = 0; i < 10; ++i)
		{
			if (isCollision(pt_UserChar->BulletMgr->UserBullet[i].m_rcCollision))
			{
				m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_HIT;
				pt_UserChar->BulletMgr->UserBullet[i].m_BulletState = USER_BULLET_STATE::BULLET_HIT;
				pt_UserChar->ScoreMgr->MyScore += 100;
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
		}
	}
}

