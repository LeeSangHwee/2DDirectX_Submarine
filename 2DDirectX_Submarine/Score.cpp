#include "Score.h"

Score::Score()
{
	srand(time(NULL));

	// Score Initialize
	m_nFrame = 0;
	m_xPos = 300.0f;
	m_yPos = 550.0f;
}

Score::~Score()
{

}

void Score::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	// Text - Number
	SetRect(&m_rcFrame[0], 0, 20, 18, 20);
	SetRect(&m_rcFrame[1], 18, 20, 18, 20);
	SetRect(&m_rcFrame[2], 36, 20, 18, 20);
	SetRect(&m_rcFrame[3], 54, 20, 18, 20);
	SetRect(&m_rcFrame[4], 72, 20, 18, 20);
	SetRect(&m_rcFrame[5], 0, 40, 18, 20);
	SetRect(&m_rcFrame[6], 18, 40, 18, 20);
	SetRect(&m_rcFrame[7], 36, 40, 18, 20);
	SetRect(&m_rcFrame[8], 54, 40, 18, 20);
	SetRect(&m_rcFrame[9], 72, 40, 18, 20);

	// Text - Score
	SetRect(&m_rcFrame[10], 0, 0, 100, 20);
}

void Score::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

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
