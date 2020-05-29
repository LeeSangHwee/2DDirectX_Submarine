#include "InGameBackGround.h"

InGameBackGround::InGameBackGround()
{
	m_xPos = 0.0f;
	m_yPos = 0.0f;
}


InGameBackGround::~InGameBackGround()
{
}

void InGameBackGround::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);
	SetRect(&m_rcDraw, 0, 0, width, height);
}

void InGameBackGround::OnDraw()
{
	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pTexture,
		&m_rcDraw,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}

void InGameBackGround::OnUpdate(float time)
{
	OnDraw();
}