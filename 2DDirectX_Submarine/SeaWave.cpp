#include "SeaWave.h"

SeaWave::SeaWave()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	m_xPos = 0.0f;
	m_yPos = 150.0f;

	srand(time(NULL));
}


SeaWave::~SeaWave()
{
}

void SeaWave::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	SetRect(&m_rcFrame[0], 0, 0, 800, 40);
	SetRect(&m_rcFrame[1], 0, 40, 800, 40);
	SetRect(&m_rcFrame[2], 0, 80, 800, 40);
	SetRect(&m_rcFrame[3], 0, 120, 800, 40);
}

void SeaWave::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_nFrame].bottom;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}

void SeaWave::OnUpdate(float time)
{
	m_interval += time;
	if (m_interval > 0.85f)
	{
		m_nFrame++;
		if (m_nFrame > 3) m_nFrame = 0;

		m_interval = 0.0f;
	}

	OnDraw();
}

BOOL SeaWave::isCollision(POINT pt)
{
	return ::PtInRect(&m_rcCollision, pt);
}