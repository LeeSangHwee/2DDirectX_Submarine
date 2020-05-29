#include "User_Bullet.h"
#include "cMP3.h"

User_Bullet::User_Bullet()
{
	srand(time(NULL));

	// Bullet Initialize
	m_nFrame = 0;
	m_interval = 0.1f;

	m_xPos = -200.0f;
	m_yPos = -200.0f;

	m_BulletState = USER_BULLET_STATE::BULLET_OFF;
	m_UpBulletState = USER_UP_BULLET_STATE::UP_OFF;
	m_DownBulletState = USER_DOWN_BULLET_STATE::DOWN_OFF;

	// Create Sound
	pt_sound = new cMP3;
}


User_Bullet::~User_Bullet()
{
}

void User_Bullet::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Top Attack
	SetRect(&m_rcFrame[0], 0, 0, 65, 63);
	SetRect(&m_rcFrame[1], 65, 0, 65, 95);
	SetRect(&m_rcFrame[2], 130, 0, 65, 33);

	// Mid Attack
	SetRect(&m_rcFrame[3], 0, 95, 77, 35);
	SetRect(&m_rcFrame[4], 77, 95, 77, 35);
	SetRect(&m_rcFrame[5], 154, 95, 77, 35);

	// Down Attack
	SetRect(&m_rcFrame[6], 0, 130, 30, 28);
	SetRect(&m_rcFrame[7], 30, 130, 30, 28);
	SetRect(&m_rcFrame[8], 60, 130, 30, 28);

	// Bullet Explosion
	SetRect(&m_rcFrame[9], 231, 0, 80, 80);
	SetRect(&m_rcFrame[10], 311, 0, 80, 80);
	SetRect(&m_rcFrame[11], 391, 0, 80, 80);
	SetRect(&m_rcFrame[12], 231, 80, 80, 80);
	SetRect(&m_rcFrame[13], 311, 80, 80, 80);
	SetRect(&m_rcFrame[14], 391, 80, 80, 80);
}

void User_Bullet::OnDraw()
{
	// Sprite 정보 전달
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	// 충돌 박스 정보 전달
	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_nFrame].bottom;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 그리는 곳
	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}

void User_Bullet::OnUpdate(float time)
{
	if(m_BulletState > USER_BULLET_STATE::BULLET_DEL)
	{
		// Bullet Frame
		m_interval += time;
		if (m_interval > 0.1f)
		{
			m_interval = 0.0f;

			switch (m_BulletState)
			{
				case USER_BULLET_STATE::BULLET_UP:
				{
					if (m_UpBulletState == USER_UP_BULLET_STATE::UP_INOCEAN)
					{
						if (m_yPos > 160.0f) m_nFrame = 1;
						else if (m_yPos <= 160.0f) m_UpBulletState = USER_UP_BULLET_STATE::UP_OUTOCEAN;
					}
					else if (m_UpBulletState == USER_UP_BULLET_STATE::UP_OUTOCEAN)
						m_nFrame = 2;
					else if (m_UpBulletState == USER_UP_BULLET_STATE::UP_START)
					{
						m_nFrame = 0;
						m_UpBulletState = USER_UP_BULLET_STATE::UP_INOCEAN;
					}
					else if (m_UpBulletState == USER_UP_BULLET_STATE::UP_OFF)
						m_UpBulletState = USER_UP_BULLET_STATE::UP_START;
				}break;
				case USER_BULLET_STATE::BULLET_MIDDLE:
				{
					m_nFrame++;
					if (m_nFrame > 5) m_nFrame = 4;
				}break;
				case USER_BULLET_STATE::BULLET_DOWN:
				{
					if (m_DownBulletState == USER_DOWN_BULLET_STATE::DOWN_LEFT)
					{
						m_nFrame--;
						if (m_nFrame < 6)
						{
							m_nFrame++;
							m_DownBulletState = USER_DOWN_BULLET_STATE::DOWN_RIGHT;
						}
					}
					else if (m_DownBulletState == USER_DOWN_BULLET_STATE::DOWN_RIGHT)
					{
						m_nFrame++;
						if (m_nFrame > 8)
						{
							m_nFrame--;
							m_DownBulletState = USER_DOWN_BULLET_STATE::DOWN_LEFT;
						}
					}
					else if (m_DownBulletState == USER_DOWN_BULLET_STATE::DOWN_OFF)
						m_DownBulletState = USER_DOWN_BULLET_STATE::DOWN_RIGHT;
				}break;
				case USER_BULLET_STATE::BULLET_HIT:
				{
					if (9 <= m_nFrame && m_nFrame < 14) m_nFrame++;
					else if (m_nFrame == 14) m_BulletState = USER_BULLET_STATE::BULLET_DEL;
					else if (m_nFrame != 9) m_nFrame = 9;
				}break;
			}
		}
		BulletMove();
		OnDraw();
	}
}

void User_Bullet::BulletMove()
{
	// Bullet Delete
	if (-35 >= m_yPos || m_yPos >= 600 || 800 <= m_xPos)
	{
		m_BulletState = USER_BULLET_STATE::BULLET_DEL;
		m_UpBulletState = USER_UP_BULLET_STATE::UP_OFF;
		m_DownBulletState = USER_DOWN_BULLET_STATE::DOWN_OFF;
	}
	else
	{
		// Bullet Move
		switch (m_BulletState)
		{
			case USER_BULLET_STATE::BULLET_UP    : m_yPos -= 0.05f; break;
			case USER_BULLET_STATE::BULLET_MIDDLE: m_xPos += 0.05f; break;
			case USER_BULLET_STATE::BULLET_DOWN  : m_yPos += 0.01f; break;
		}
	}
}

BOOL User_Bullet::isCollision(RECT pt_rect)
{
	RECT rcTemp;
	return ::IntersectRect(&rcTemp, &m_rcCollision, &pt_rect);
}

void User_Bullet::ChangeBulletImg(USER_BULLET_STATE type)
{
	if (m_BulletState == type)
		return;

	m_BulletState = type;
}