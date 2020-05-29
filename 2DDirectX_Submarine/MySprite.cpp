#include "MySprite.h"



CMySprite::CMySprite()
{
	m_pTexture = NULL;
	m_pSprite = NULL;
}


CMySprite::~CMySprite()
{
	if (m_pSprite != NULL)
		m_pSprite->Release();

	if (m_pTexture != NULL)
		m_pTexture->Release();
}

void CMySprite::Create(LPDIRECT3DDEVICE9 pDevice, char *filename, int width, int height)
{
	D3DXIMAGE_INFO d3dxImageInfo;

	D3DXCreateTextureFromFileEx(pDevice,
		filename,
		width, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
		height, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
		1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_COLORVALUE(1.0f, 0.0f, 1.0f, 1.0f),
		&d3dxImageInfo,
		NULL,
		&m_pTexture);


	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	D3DXCreateSprite(pDevice, &m_pSprite);
}
