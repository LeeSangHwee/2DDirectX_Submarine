#include "UserBulletMgr.h"
#include "UserCharacter.h"
#include "User_Bullet.h"
#include "cMP3.h"

UserBulletMgr::UserBulletMgr()
{
	m_BulletCount = 10;
	m_interval = 5.0f;

	mgr_BulletState = USER_BULLET_STATE::BULLET_OFF;
	UserBullet = new User_Bullet[m_BulletCount];
}

UserBulletMgr::~UserBulletMgr()
{

}

void UserBulletMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < 10; ++i) UserBullet[i].Create(pDevice, "Sprite/User_Bullet.bmp", 471, 160);
}

void UserBulletMgr::OnUpdate(float _time, float UserPosX, float UserPosY)
{
	for (int i = 0; i < 10; ++i)
	{
		if (UserBullet[i].m_BulletState == USER_BULLET_STATE::BULLET_OFF && mgr_BulletState != USER_BULLET_STATE::BULLET_OFF)
		{
			// Load Sound
			UserBullet[i].pt_sound->LoadSound("Sounds/rlaunch.wav");
			UserBullet[i].pt_sound->RunSounds();

			// Create Bullet		
			if (mgr_BulletState == USER_BULLET_STATE::BULLET_UP)
			{
				UserBullet[i].SetBullet((unsigned short)USER_BULLET_STATE::BULLET_UP, USER_BULLET_STATE::BULLET_UP, UserPosY - 5, UserPosX + 34);
				mgr_BulletState = USER_BULLET_STATE::BULLET_DOWN;
				m_BulletCount--;
				continue;
			}
			else if (mgr_BulletState == USER_BULLET_STATE::BULLET_DOWN)
			{
				UserBullet[i].SetBullet((unsigned short)USER_BULLET_STATE::BULLET_DOWN, USER_BULLET_STATE::BULLET_DOWN, UserPosY + 67, UserPosX + 48);
				mgr_BulletState = USER_BULLET_STATE::BULLET_OFF;				
			}
			else if (mgr_BulletState == USER_BULLET_STATE::BULLET_MIDDLE)
				UserBullet[i].SetBullet((unsigned short)USER_BULLET_STATE::BULLET_MIDDLE, USER_BULLET_STATE::BULLET_MIDDLE, UserPosY + 33, UserPosX + 120);

			mgr_BulletState = USER_BULLET_STATE::BULLET_OFF;
			m_BulletCount--;
			break;
		}
		else if (UserBullet[i].m_BulletState == USER_BULLET_STATE::BULLET_DEL)
		{
			if (10 > m_BulletCount)
			{
				// Delete Bullet
				UserBullet[i].m_BulletState = USER_BULLET_STATE::BULLET_OFF;
				UserBullet[i].m_nFrame = 0;
				UserBullet[i].m_interval = 0.1f;
				
				UserBullet[i].m_xPos = -200.0f;
				UserBullet[i].m_yPos = -200.0f;
				UserBullet[i].OnDraw();
				UserBullet[i].pt_sound->StopSounds();
				m_BulletCount++;
			}
		}
		// Update & Draw Bullet
		else if (UserBullet[i].m_BulletState != USER_BULLET_STATE::BULLET_OFF)
			UserBullet[i].OnUpdate(_time);
	}
}

void UserBulletMgr::UserBulletOnDraw()
{
	for (int i = 0; i < 10; ++i)
		UserBullet[i].OnDraw();
}
