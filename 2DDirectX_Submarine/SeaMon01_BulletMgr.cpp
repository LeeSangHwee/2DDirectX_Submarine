#include "SeaMon01_BulletMgr.h"
#include "SeaMon01_Bullet.h"
#include "UserCharacter.h"
#include "cMP3.h"

SeaMon01_BulletMgr::SeaMon01_BulletMgr()
{
	// Monster Bullet Initialize
	m_BulletCount = 0;
	m_interval = 1.0f;

	pt_MonBullet = new SeaMon01_Bullet[15];
}

SeaMon01_BulletMgr::~SeaMon01_BulletMgr()
{

}

void SeaMon01_BulletMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < 15; ++i) pt_MonBullet[i].Create(pDevice, "Sprite/SeaMon01_Bullet.bmp", 275, 140);
}

void SeaMon01_BulletMgr::OnUpdate(float _time, float MonPosX, float MonPosY, class UserCharacter* _UserChar, SEA_MONSTER01_STATE m_MonsterState)
{
	m_interval += _time;

	for (int i = 0; i < 15; ++i)
	{
		// SeaMon01_Bullet - User info
		pt_MonBullet[i].SetptUser(_UserChar);

		if (pt_MonBullet[i].m_BulletState == SEAMON01_BULLET_STATE::SEAMON01_BULLET_OFF)
		{
			if (m_interval >= 1.5f)
			{
				m_interval = 0.0f;

				// Create Bullet
				if (0 <= m_BulletCount && m_BulletCount < 2)
				{
					m_BulletCount++;
					pt_MonBullet[i].SetBullet((unsigned short)SEAMON01_BULLET_STATE::SEAMON01_BULLET_MID, SEAMON01_BULLET_STATE::SEAMON01_BULLET_MID, MonPosY + 30, MonPosX - 50);
					break;
				}
				else if(m_BulletCount == 2)
				{
					if (MonPosY > _UserChar->GetPosY() + 34)
						pt_MonBullet[i].SetBullet((unsigned short)SEAMON01_BULLET_STATE::SEAMON01_BULLET_UP, SEAMON01_BULLET_STATE::SEAMON01_BULLET_UP, MonPosY, MonPosX + 55);

					m_BulletCount = 0;
					break;
				}
			}
		}
		// Delete Bullet
		else if (pt_MonBullet[i].m_BulletState == SEAMON01_BULLET_STATE::SEAMON01_BULLET_DEL)
		{
			pt_MonBullet[i].m_BulletState = SEAMON01_BULLET_STATE::SEAMON01_BULLET_OFF;
			pt_MonBullet[i].pt_sound->StopSounds();

			pt_MonBullet[i].m_nFrame = -3;
			pt_MonBullet[i].m_interval = 0.0f;
			
			pt_MonBullet[i].m_xPos = 0.0f;
			pt_MonBullet[i].m_yPos = 0.0f;
		}
		else if (pt_MonBullet[i].m_BulletState != SEAMON01_BULLET_STATE::SEAMON01_BULLET_OFF)
			pt_MonBullet[i].OnUpdate(_time);
	}
}