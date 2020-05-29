#include "AirMon01_BulletMgr.h"
#include "AirMon01_Bullet.h"
#include "UserCharacter.h"
#include "cMP3.h"

AirMon01_BulletMgr::AirMon01_BulletMgr()
{
	// Monster Bullet Initialize
	m_BulletCount = 0;
	m_interval = 0.0f;

	pt_MonBullet = new AirMon01_Bullet[10];
}

AirMon01_BulletMgr::~AirMon01_BulletMgr()
{

}

void AirMon01_BulletMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < 10; ++i) pt_MonBullet[i].Create(pDevice, "Sprite/AirMon01_Bullet.bmp", 78, 80);
}

void AirMon01_BulletMgr::OnUpdate(float _time, float MonPosX, float MonPosY, AIR_MONSTER01_STATE _MosnterState, class UserCharacter* _UserChar)
{
	m_interval += _time;
	for (int i = 0; i < 10; ++i)
	{
		// AirMon01_Bullet - User info
		pt_MonBullet[i].SetptUser(_UserChar);

		if (pt_MonBullet[i].m_BulletState == AIRMON01_BULLET_STATE::AIRMON01_BULLET_OFF)
		{
			if (m_interval >= 5.0f)
			{
				// Create Bullet
				if (_MosnterState == AIR_MONSTER01_STATE::AIR_MONSTER01_LATTACK)
				{
					m_interval = 0.0f;
					pt_MonBullet[i].SetBullet((unsigned short)AIRMON01_BULLET_STATE::AIRMON01_BULLET_LEFT, AIRMON01_BULLET_STATE::AIRMON01_BULLET_LEFT, MonPosY + 60, MonPosX + 30);
					m_BulletCount++;
					break;
				}
				else if (_MosnterState == AIR_MONSTER01_STATE::AIR_MONSTER01_RATTACK)
				{
					m_interval = 0.0f;
					pt_MonBullet[i].SetBullet((unsigned short)AIRMON01_BULLET_STATE::AIRMON01_BULLET_RIGHT, AIRMON01_BULLET_STATE::AIRMON01_BULLET_RIGHT, MonPosY + 60 ,MonPosX + 70);
					m_BulletCount++;
					break;
				}
			}
		}
		// Delete Bullet
		else if (pt_MonBullet[i].m_BulletState == AIRMON01_BULLET_STATE::AIRMON01_BULLET_DEL)
		{
			pt_MonBullet[i].m_BulletState = AIRMON01_BULLET_STATE::AIRMON01_BULLET_OFF;
			pt_MonBullet[i].pt_sound->StopSounds();

			pt_MonBullet[i].m_nFrame = -1;
			pt_MonBullet[i].m_interval = 0.5f;

			pt_MonBullet[i].m_xPos = 0.0f;
			pt_MonBullet[i].m_yPos = 0.0f;

			m_BulletCount--;
		}			
		// Update & Draw Bullet
		else if (pt_MonBullet[i].m_BulletState != AIRMON01_BULLET_STATE::AIRMON01_BULLET_OFF)
			pt_MonBullet[i].OnUpdate(_time);
	}
}