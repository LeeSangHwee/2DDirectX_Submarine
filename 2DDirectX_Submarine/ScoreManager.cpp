#include "ScoreManager.h"
#include "Score.h"

ScoreManager::ScoreManager()
{
	MyScore = 0;
	m_interval = 0.0f;

	pt_score = new Score[7];
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Create(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < 7; ++i) pt_score[i].Create(pDevice, "Sprite/Score_Text.bmp", 100, 60);

	// Initialize Score
	pt_score[0].SetScore(10, 530.0f, 250.0f);
}

void ScoreManager::OnUpdate(float time)
{
	// Score Draw
	pt_score[0].OnDraw();
	for (int i = 1; i < 7; ++i)
	{
		pt_score[i].SetScore((unsigned short)(MyScore / (int)pow(10, i - 1)) % 10, 530.0f, 510.0f - (20 * i));
		pt_score[i].OnDraw();
	}
}
