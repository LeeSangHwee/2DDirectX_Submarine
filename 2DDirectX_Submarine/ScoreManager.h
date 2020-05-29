#pragma once
#include "Headers.h"

class ScoreManager
{
public:
	int MyScore;

private:
	float m_interval;

private:
	class Score* pt_score;

public:
	ScoreManager();
	~ScoreManager();

	void Create(LPDIRECT3DDEVICE9 pDevice);
	void OnUpdate(float time);
};
