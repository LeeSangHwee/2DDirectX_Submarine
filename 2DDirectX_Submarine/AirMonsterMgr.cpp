#include "AirMonsterMgr.h"
#include "AirMon01_Char.h"
#include "cMP3.h"

AirMonsterMgr::AirMonsterMgr()
{
	m_AirMonCount = 5;
	m_interval = 30.0f;
	m_xPos = 500.0f;
	m_yPos = -65.0f;

	AirMon_01 = new AirMon01_Char[m_AirMonCount];
}

AirMonsterMgr::~AirMonsterMgr()
{

}

void AirMonsterMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < 5; ++i) AirMon_01[i].Create(pDevice, "Sprite/AirMon01_Char.bmp", 1450, 223);
}

void AirMonsterMgr::OnUpdate(float _time, class UserCharacter* pt_UserChar)
{
	m_interval += _time;

	for (int i = 0; i < 5; ++i)
	{
		// AirMon01 - User info
		AirMon_01[i].SetptUser(pt_UserChar);

		if (AirMon_01[i].m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_OFF)
		{
			if (m_interval >= 30.0f && 0 < m_AirMonCount)
			{
				// Create monster
				m_interval = 0.0f;

				AirMon_01[i].SetAirMon01((unsigned short)AIR_MONSTER01_STATE::AIR_MONSTER01_DOWN, AIR_MONSTER01_STATE::AIR_MONSTER01_DOWN, m_yPos, m_xPos);
				m_AirMonCount--;
			}
		}
		else if (AirMon_01[i].m_MonsterState == AIR_MONSTER01_STATE::AIR_MONSTER01_DEL)
		{
			if (5 > m_AirMonCount)
			{
				m_interval = 30.0f;

				// Delete Monster
				AirMon_01[i].m_MonsterState = AIR_MONSTER01_STATE::AIR_MONSTER01_OFF;
				AirMon_01[i].pt_sound->StopSounds();

				AirMon_01[i].m_nFrame = -1;
				
				AirMon_01[i].m_interval = 0.5f;
				AirMon_01[i].m_BulletInterval = 0.0f;
				
				AirMon_01[i].m_xPos = 500.0f;
				AirMon_01[i].m_yPos = -65.0f;

				m_AirMonCount++;
			}
		}
		else AirMon_01[i].OnUpdate(_time);
	}
}

inline void AirMonsterMgr::AirMonOnDraw()
{
	for (int i = 0; i < 5; ++i)
		AirMon_01[i].OnDraw();
}
