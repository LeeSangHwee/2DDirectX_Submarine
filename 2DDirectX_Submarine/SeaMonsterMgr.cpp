#include "SeaMonsterMgr.h"
#include "SeaMon01_Char.h"
#include "UserCharacter.h"
#include "cMP3.h"

SeaMonsterMgr::SeaMonsterMgr()
{
	m_SeaMonCount = 10;
	m_interval = 2.0f;
	m_xPos = 801.0f;
	m_yPos = 0.0f;
	
	SeaMon_01 = new SeaMon01_Char[m_SeaMonCount];
}

SeaMonsterMgr::~SeaMonsterMgr()
{

}

void SeaMonsterMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < 10; ++i) SeaMon_01[i].Create(pDevice, "Sprite/SeaMon01_Char.bmp", 510, 240);
}

void SeaMonsterMgr::OnUpdate(float _time, class UserCharacter* pt_UserChar)
{	
	m_interval += _time;

	for (int i = 0; i < 10; ++i) 
	{
		// SeaMon01 - User info
		SeaMon_01[i].SetptUser(pt_UserChar);

		if (SeaMon_01[i].m_MonsterState == SEA_MONSTER01_STATE::SEA_MONSTER01_OFF)
		{
			if (m_interval >= 3.0f && 0 < m_SeaMonCount)
			{
				// Create monster
				m_interval = 0.0f;
				m_yPos = (rand() % 300) + 200;

				SeaMon_01[i].SetSeaMon01((unsigned short)SEA_MONSTER01_STATE::SEA_MONSTER01_IDLE, SEA_MONSTER01_STATE::SEA_MONSTER01_IDLE, m_yPos, m_xPos);
				m_SeaMonCount--;
			}
		}
		else if (SeaMon_01[i].m_MonsterState == SEA_MONSTER01_STATE::SEA_MONSTER01_DEL)
		{
			if (10 > m_SeaMonCount)
			{
				// Delete Monster
				SeaMon_01[i].m_MonsterState = SEA_MONSTER01_STATE::SEA_MONSTER01_OFF;
				SeaMon_01[i].pt_sound->StopSounds();

				SeaMon_01[i].m_nFrame = -1;
				SeaMon_01[i].m_interval = 0.3f;

				SeaMon_01[i].m_xPos = 0.0f;
				SeaMon_01[i].m_yPos = 0.0f;

				m_SeaMonCount++;
			}
		}

		SeaMon_01[i].OnUpdate(_time);
	}
}

void SeaMonsterMgr::SeaMonOnDraw()
{
	for (int i = 0; i < 10; ++i)
		SeaMon_01[i].OnDraw();
}
