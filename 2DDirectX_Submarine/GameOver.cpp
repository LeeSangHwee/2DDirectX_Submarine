#include "GameOver.h"
#include "cMP3.h"

GameOver::GameOver()
{
	srand(time(NULL));

	// GameOver Initialize
	m_xPos = 200.0f;
	m_yPos = 200.0f;

	// Create Sound
	pt_sound = new cMP3;
}

GameOver::~GameOver()
{

}

void GameOver::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Load Sound
	pt_sound->LoadBGM("Sounds/VictorySmall.wav");

	// Text - Number
	SetRect(&m_rcFrame, 0, 0, 400, 200);
}

void GameOver::OnDraw()
{
	RECT srcRect = m_rcFrame;
	srcRect.bottom = srcRect.top + m_rcFrame.bottom;
	srcRect.right = srcRect.left + m_rcFrame.right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}
