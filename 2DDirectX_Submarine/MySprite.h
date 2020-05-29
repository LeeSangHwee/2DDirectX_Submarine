#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class CMySprite
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXSPRITE       m_pSprite;

public:
	CMySprite();
	virtual ~CMySprite();

	virtual void Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height);
	virtual void OnDraw() {}
	virtual void OnUpdate(float time) {}
};

