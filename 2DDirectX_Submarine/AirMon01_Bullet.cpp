#include "AirMon01_Bullet.h"
#include "UserCharacter.h"
#include "UserBulletMgr.h"
#include "User_Bullet.h"
#include "ScoreManager.h"
#include "cMP3.h"

AirMon01_Bullet::AirMon01_Bullet()
{
	srand(time(NULL));

	m_nFrame = -1;
	m_CollisionInterval = 0.0f;
	m_interval = 0.5f;

	m_xPos = 0.0f;
	m_yPos = 0.0f;

	m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_OFF;

	// Create Sound
	pt_sound = new cMP3;
}


AirMon01_Bullet::~AirMon01_Bullet()
{

}

void AirMon01_Bullet::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Left Move Attack
	SetRect(&m_rcFrame[0], 0, 0, 33, 37);
	SetRect(&m_rcFrame[1], 33, 0, 25, 40);
	SetRect(&m_rcFrame[2], 58, 0, 20, 32);

	// Right Move Attack
	SetRect(&m_rcFrame[3], 0, 40, 33, 37);
	SetRect(&m_rcFrame[4], 33, 40, 25, 40);
	SetRect(&m_rcFrame[5], 58, 40, 20, 32);

	// Blank
	SetRect(&m_rcFrame[6], 0, 0, 1, 1);
}

void AirMon01_Bullet::OnDraw()
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

void AirMon01_Bullet::OnUpdate(float time)
{
	// Collision
	if (pt_UserChar->m_UserState < USER_STATE::USER_HIT)
		BulletCollision(time);

	if (m_BulletState > AIRMON01_BULLET_STATE::AIRMON01_BULLET_DEL)
	{
		// Bullet Frame
		m_interval += time;
		if (m_interval > 0.4f)
		{
			m_interval = 0.0f;

			switch (m_BulletState)
			{
				case AIRMON01_BULLET_STATE::AIRMON01_BULLET_LEFT :if (m_nFrame < 2)  m_nFrame++; break;
				case AIRMON01_BULLET_STATE::AIRMON01_BULLET_RIGHT:if (m_nFrame < 5)  m_nFrame++; break;
				case AIRMON01_BULLET_STATE::AIRMON01_BULLET_HIT:
				{
					if (m_nFrame != 6) m_nFrame = 6;
					m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_DEL; break;
				}
			}
		}

		BulletMove();
		OnDraw();
	}
}

void AirMon01_Bullet::BulletMove()
{	
	if (600.0f <= m_yPos) // Bullet Delete
		m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_DEL;
	else // Bullet Move
	{		
		switch (m_BulletState)
		{
			case AIRMON01_BULLET_STATE::AIRMON01_BULLET_LEFT:
			{
				if (m_nFrame != 2) m_xPos -= 0.03;
				m_yPos += 0.05f;
			}break;
			case AIRMON01_BULLET_STATE::AIRMON01_BULLET_RIGHT:
			{
				if (m_nFrame != 5) m_xPos += 0.03;
				m_yPos += 0.05f;
			}break;
		}
	}
}

BOOL AirMon01_Bullet::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void AirMon01_Bullet::BulletCollision(float time)
{
	m_CollisionInterval += time;
	if (m_CollisionInterval > 0.2f)
	{
		m_CollisionInterval = 0.0f;

		for (int i = 0; i < 10; ++i)
		{
			if (isCollision(pt_UserChar->BulletMgr->UserBullet[i].m_rcCollision))
			{
				m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_HIT;
				pt_UserChar->BulletMgr->UserBullet[i].m_BulletState = USER_BULLET_STATE::BULLET_HIT;
				pt_UserChar->ScoreMgr->MyScore += 2;
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
			else if (isCollision(pt_UserChar->m_rcCollision))
			{
				m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_HIT;
				pt_UserChar->m_UserState = USER_STATE::USER_HIT;
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
		}
	}
}