#include "AirMon01_Char.h"
#include "AirMon01_BulletMgr.h"
#include "UserCharacter.h"
#include "UserBulletMgr.h"
#include "User_Bullet.h"
#include "ScoreManager.h"
#include "cMP3.h"

AirMon01_Char::AirMon01_Char()
{
	// Monster Initialize
	m_nFrame = -1;

	m_interval = 0.5f;
	m_BulletInterval = 0.0f;
	m_CollisionInterval = 0.0f;

	m_xPos = 500.0f;
	m_yPos = -65.0f;

	m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_OFF;

	// Create Bullet Manager
	pt_BulletMgr = new AirMon01_BulletMgr;

	// Create Sound
	pt_sound = new cMP3;
}


AirMon01_Char::~AirMon01_Char()
{
}

void AirMon01_Char::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Right - Attack
	SetRect(&m_rcFrame[0], 0, 0, 117, 103);
	SetRect(&m_rcFrame[1], 117, 0, 128, 80);
	// Right - Move
	SetRect(&m_rcFrame[2], 245, 0, 138, 63);
	// Right - Rotate
	SetRect(&m_rcFrame[3], 383, 0, 113, 63);
	SetRect(&m_rcFrame[4], 496, 0, 95, 63);
	SetRect(&m_rcFrame[5], 591, 0, 80, 63);

	// Down
	SetRect(&m_rcFrame[6], 671, 0, 108, 65);

	// Left - Rotate
	SetRect(&m_rcFrame[7], 779, 0, 80, 63);
	SetRect(&m_rcFrame[8], 859, 0, 95, 63);
	SetRect(&m_rcFrame[9], 954, 0, 113, 63);
	// Left - Move
	SetRect(&m_rcFrame[10], 1067, 0, 138, 63);
	// Left - Attack
	SetRect(&m_rcFrame[11], 1205, 0, 128, 80);
	SetRect(&m_rcFrame[12], 1333, 0, 117, 103);

	// Enemy Explosion
	SetRect(&m_rcFrame[13], 0, 103, 120, 120);
	SetRect(&m_rcFrame[14], 120, 103, 120, 120);
	SetRect(&m_rcFrame[15], 240, 103, 120, 120);
	SetRect(&m_rcFrame[16], 360, 103, 120, 120);
	SetRect(&m_rcFrame[17], 480, 103, 120, 120);
	SetRect(&m_rcFrame[18], 600, 103, 120, 120);

	pt_BulletMgr->Create(pDevice);
}

void AirMon01_Char::OnDraw()
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

void AirMon01_Char::OnUpdate(float time)
{
	// Collision
	BulletCollision(time);

	m_interval += time;
	m_BulletInterval += time;

	if (m_interval > 0.1f)
	{
		m_interval = 0.0f;

		// Move
		if (m_yPos <= 30.0f)
			m_yPos += 3.0f;
		else if(m_nFrame == 11 && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LEFT)
			m_xPos -= 5.0f;
		else if (m_nFrame == 1 && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_RIGHT)
			m_xPos += 5.0f;

		// Change Frame
		if (m_yPos > 30.0f && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_DOWN)
		{
			if (m_nFrame != 11) m_nFrame++;
			else if (m_nFrame == 11) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_LEFT;
		}
		else if (m_xPos <= 0.0f && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LEFT)
		{
			if (m_nFrame != 1) m_nFrame--;
			else if (m_nFrame == 1) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_RIGHT;
		}
		else if (m_xPos < 500.0f && m_BulletInterval > 5.0f && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LEFT)
		{
			m_BulletInterval = 0.0f;
			m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_LATTACK;		
		}
		else if (m_xPos < 500.0f && m_BulletInterval > 5.0f && m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_RIGHT)
		{
			m_BulletInterval = 0.0f;
			m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_RATTACK;
		}
		else if (m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LATTACK)
		{
			if (m_nFrame != 12) m_nFrame++;
			else if (m_nFrame == 12) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_LRELOAD;
		}
		else if (m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_RATTACK)
		{
			if (m_nFrame != 0) m_nFrame--;
			else if (m_nFrame == 0) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_RRELOAD;
		}
		else if (m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LRELOAD)
		{
			if (m_nFrame != 11) m_nFrame--;
			else if (m_nFrame == 11) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_LEFT;
		}
		else if (m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_RRELOAD)
		{
			if (m_nFrame != 1) m_nFrame++;
			else if (m_nFrame == 1) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_RIGHT;
		}
		else if (m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_HIT)
		{
			if (13 <= m_nFrame && m_nFrame < 18) m_nFrame++;
			else if (m_nFrame == 18) m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_DEL;
			else if (m_nFrame != 13) m_nFrame = 13;
		}
	}
	OnDraw();
	pt_BulletMgr->OnUpdate(time, m_xPos, m_yPos, m_MonsterState, pt_UserChar);
}

BOOL AirMon01_Char::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void AirMon01_Char::BulletCollision(float time)
{
	m_CollisionInterval += time;
	if (m_CollisionInterval > 0.2f)
	{
		m_CollisionInterval = 0.0f;

		for (int i = 0; i < 10; ++i)
		{
			if (isCollision(pt_UserChar->BulletMgr->UserBullet[i].m_rcCollision))
			{
				m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_HIT;
				pt_UserChar->BulletMgr->UserBullet[i].m_BulletState = USER_BULLET_STATE::BULLET_HIT;
				pt_UserChar->BulletMgr->UserBullet[i].pt_sound->StopBGM();
				pt_UserChar->ScoreMgr->MyScore += 150;				
				// Load Sound
				pt_sound->LoadSound("Sounds/boom9.wav");
				pt_sound->RunSounds();
			}
		}
	}
}