#include "SeaMon01_Bullet.h"
#include "UserCharacter.h"
#include "UserBulletMgr.h"
#include "User_Bullet.h"
#include "ScoreManager.h"
#include "cMP3.h"

SeaMon01_Bullet::SeaMon01_Bullet()
{
	srand(time(NULL));

	m_nFrame = -3;
	m_CollisionInterval = 0.0f;
	m_interval = 0.0f;

	m_xPos = 0.0f;
	m_yPos = 0.0f;

	m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_OFF;

	// Create Sound
	pt_sound = new cMP3;
}


SeaMon01_Bullet::~SeaMon01_Bullet()
{

}

void SeaMon01_Bullet::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Top Attack
	SetRect(&m_rcFrame[0], 0, 0, 60, 55);
	SetRect(&m_rcFrame[1], 60, 0, 40, 100);
	SetRect(&m_rcFrame[2], 100, 0, 40, 100);

	// Mid Attack
	SetRect(&m_rcFrame[3], 0, 100, 75, 40);
	SetRect(&m_rcFrame[4], 75, 100, 100, 25);
	SetRect(&m_rcFrame[5], 175, 100, 100, 25);

	// Blank
	SetRect(&m_rcFrame[6], 0, 0, 1, 1);
}

void SeaMon01_Bullet::OnDraw()
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

void SeaMon01_Bullet::OnUpdate(float time)
{
	if(m_BulletState > SEAMON01_BULLET_STATE::SEAMON01_BULLET_DEL)
	{
		// Collision
		if(pt_UserChar->m_UserState < USER_STATE::USER_HIT)
			BulletCollision(time);

		// Bullet Frame
		m_interval += time;
		if (m_interval > 0.3f)
		{
			m_interval = 0.0f;
			m_nFrame++;

			switch (m_BulletState)
			{
				case SEAMON01_BULLET_STATE::SEAMON01_BULLET_UP: if (m_nFrame > 2) m_nFrame = 1; break;
				case SEAMON01_BULLET_STATE::SEAMON01_BULLET_MID:if (m_nFrame > 5) m_nFrame = 4; break;
				case SEAMON01_BULLET_STATE::SEAMON01_BULLET_HIT:
				{
					if (m_nFrame != 6) m_nFrame = 6;
					m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_DEL; break;
				}
			}
		}
		
		BulletMove();		
		OnDraw();
	}
} 

void SeaMon01_Bullet::BulletMove()
{
	// Bullet Delete
	if (150.0f >= m_yPos || -110.0f >= m_xPos)
		m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_DEL;
	else
	{
		// Bullet Move
		switch (m_BulletState)
		{
			case SEAMON01_BULLET_STATE::SEAMON01_BULLET_UP: m_yPos -= 0.05f; break;
			case SEAMON01_BULLET_STATE::SEAMON01_BULLET_MID:m_xPos -= 0.05f; break;
		}
	}
}

BOOL SeaMon01_Bullet::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void SeaMon01_Bullet::BulletCollision(float time)
{
	m_CollisionInterval += time;
	if (m_CollisionInterval > 0.2f)
	{
		m_CollisionInterval = 0.0f;

		for (int i = 0; i < 10; ++i)
		{
			if (isCollision(pt_UserChar->BulletMgr->UserBullet[i].m_rcCollision))
			{
				m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_HIT;
				pt_UserChar->BulletMgr->UserBullet[i].m_BulletState = USER_BULLET_STATE::BULLET_HIT;
				pt_UserChar->ScoreMgr->MyScore += 1;
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
			else if (isCollision(pt_UserChar->m_rcCollision))
			{
				m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_HIT;
				pt_UserChar->m_UserState = USER_STATE::USER_HIT;
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
		}
	}
}
