#include "LoadingDisplay.h"

LoadingDisplay::LoadingDisplay()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	m_xPos = 0.0f;
	m_yPos = 0.0f;

	LoadCount = 0;

	srand(time(NULL));
}


LoadingDisplay::~LoadingDisplay()
{
}

void LoadingDisplay::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	SetRect(&m_rcFrame[0], 0, 0, 800, 600);
	SetRect(&m_rcFrame[1], 800, 0, 800, 600);
	SetRect(&m_rcFrame[2], 0, 600, 800, 600);
	SetRect(&m_rcFrame[3], 800, 600, 800, 600);
	SetRect(&m_rcFrame[4], 0, 1200, 800, 600);
	SetRect(&m_rcFrame[5], 800, 1200, 800, 600);
}

void LoadingDisplay::OnDraw()
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

void LoadingDisplay::OnUpdate(float time)
{
	m_interval += time;
	if (m_interval > 0.11f)
	{
		m_nFrame++;
		if (m_nFrame > 4)
		{
			m_nFrame = 0;
			LoadCount++;
		}

		m_interval = 0.0f;
	}

	OnDraw();
}
