#include "ExitButton.h"

ExitButton::ExitButton()
{
	m_eChageButton = COL_EXIT_OFF;
	PosX = 284;
	PosY = 450;
}


ExitButton::~ExitButton()
{
}

void ExitButton::Create(LPDIRECT3DDEVICE9 pDevice, char* filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	SetRect(&m_ButtonImg[0], 232, 0, 232, 95);
	SetRect(&m_ButtonImg[1], 0, 0, 232, 95);
}

void ExitButton::OnDraw()
{
	RECT srcRect = m_ButtonImg[(int)m_eChageButton];
	srcRect.bottom = srcRect.top + m_ButtonImg[(int)m_eChageButton].bottom;
	srcRect.right = srcRect.left + m_ButtonImg[(int)m_eChageButton].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(PosX, PosY, 0.0f);

	m_rcCollision.left = (int)PosX;
	m_rcCollision.top = (int)PosY;
	m_rcCollision.right = m_rcCollision.left + m_ButtonImg[(int)m_eChageButton].right;
	m_rcCollision.bottom = m_rcCollision.top + m_ButtonImg[(int)m_eChageButton].bottom;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSprite->End();
}

void ExitButton::OnUpdate(float time)
{
	OnDraw();
}

BOOL ExitButton::isCollision(POINT pt)
{
	return ::PtInRect(&m_rcCollision, pt);
}

void ExitButton::ChangeButtonImg(EXIT_BUTTON_IMG type)
{
	if (m_eChageButton == type)
		return;

	m_eChageButton = type;
}