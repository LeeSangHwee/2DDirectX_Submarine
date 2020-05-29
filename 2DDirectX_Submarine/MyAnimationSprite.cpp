#include "MyAnimationSprite.h"
#include <time.h>


// 초기화
CMyAnimationSprite::CMyAnimationSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	m_xPos = 400.0f;
	m_yPos = 300.0f;
	m_moveInterval = 0.0f;
	m_moveDuration = 2.0f;

	m_flyType = FLYTYPE::FLY;

	srand(time(NULL));
}


CMyAnimationSprite::~CMyAnimationSprite()
{
}

// Sprite 객체 생성
void CMyAnimationSprite::Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height)
{
	CMySprite::Create(pDevice, filename, width, height);

	SetRect(&m_rcFrame[0], 16, 12, 130, 112);
	SetRect(&m_rcFrame[1], 172, 12, 130, 112);
	SetRect(&m_rcFrame[2], 329, 12, 130, 112);
	SetRect(&m_rcFrame[3], 16, 145, 130, 112);
	SetRect(&m_rcFrame[4], 174, 145, 130, 112);
	SetRect(&m_rcFrame[5], 16, 276, 130, 112);

	SetRect(&m_rcFrame[6], 159, 259, 113, 102);
}

void CMyAnimationSprite::OnDraw()
{
	// Sprite 정보 전달
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(m_xPos, m_yPos, 0.0f);

	// 충돌 박스 정보 전달
	m_rcCollision.left = (int)m_xPos;
	m_rcCollision.top = (int)m_yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_nFrame].bottom;

	
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 그리는 곳
	m_pSprite->Draw(m_pTexture,
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pSprite->End();
}

// 계속 업데이트
void CMyAnimationSprite::OnUpdate(float time)
{
	if (m_flyType == FLYTYPE::FLY)
	{
		m_interval += time;
		if (m_interval > 0.01f)
		{
			m_nFrame++;
			if (m_nFrame > 5) m_nFrame = 0;

			m_interval = 0.0f;
		}

		m_moveInterval += time;
		if (m_moveInterval >= m_moveDuration)
		{
			m_moveInterval = 0.0f;
			m_xPos = (float)(rand() % 900);
			m_yPos = (float)(rand() % 720);
		}
	}
	else
	{
		m_nFrame = 6;
	}

	OnDraw();
}

// 충돌 시 호출
BOOL CMyAnimationSprite::isCollision(POINT pt)
{
	return ::PtInRect(&m_rcCollision, pt);
}

// Sprite 바꾸기
void CMyAnimationSprite::ChangeFlyType(FLYTYPE type)
{
	if (m_flyType == type)
		return;

	m_flyType = type;
}