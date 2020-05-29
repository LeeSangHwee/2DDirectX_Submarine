//-----------------------------------------------------------------------------
//           Name: dx9_d3dxsprite.cpp
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: This sample demonstrates how to create a animated 2D sprite 
//                 using D3DXSprite which is hardware accelerated and fully 
//                 compatible with 3D generated content.
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

// Default Header
#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "resource.h"
// Main Menu Header
#include "MenuBackGround.h"
#include "StartButton.h"
#include "ExitButton.h"
// Loading Header
#include "LoadingDisplay.h"
// InGame Header
#include "InGameBackGround.h"
#include "SeaWave.h"
#include "UserCharacter.h"
#include "SeaMonsterMgr.h"
#include "AirMonsterMgr.h"
#include "GameOver.h"
// Sounds
#include "cMP3.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;

// Game State Enum
enum GAMESTATE
{
	MAIN = 0,
	LOADING	,
	INGAME	,
	EXIT	,
};

GAMESTATE g_GameState = MAIN;

float  g_fElpasedTime;
double g_dCurTime;
double g_dLastTime;
float  g_fMoveSpeed = 50.0f;
float  g_fSpinX = 0.0f;
float  g_fSpinY = 0.0f;
float  g_ExitTime = 0.0f;

// MainMenu Class
MenuBackGround		g_MenuBackGround;
StartButton			g_StartButton;
ExitButton			g_ExitButton;
// Loading Class
LoadingDisplay		g_Loading;
// InGame Class
InGameBackGround	g_GameBackGround;
SeaWave				g_SeaWave;
UserCharacter		g_UserChar;
SeaMonsterMgr		g_SeaMonMgr;
AirMonsterMgr		g_AirMonMgr;
GameOver			g_GameOver;
//Sounds
cMP3				g_sounds[2];

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_TEX1 )

struct Vertex
{
    float x, y, z;
    float tu, tv;
};

Vertex g_quadVertices[] =
{
	{-1.0f, 1.0f, 0.0f,  0.0f,0.0f },
	{ 1.0f, 1.0f, 0.0f,  1.0f,0.0f },
	{-1.0f,-1.0f, 0.0f,  0.0f,1.0f },
	{ 1.0f,-1.0f, 0.0f,  1.0f,1.0f }
};


#define WINDOW_WIDTH	800
#define	WINDOW_HEIGHT   600


//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void CreateSprite(void);
void shutDown(void);
void render(void);
void ChangeImg(POINT* _ptLastMousePosit);
bool ColSystem();

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));
    
	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
		                     "Direct3D (DX9) - Creating 2D Sprites with D3DXSPRITE",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		{
			g_dCurTime     = timeGetTime();
			g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			g_dLastTime    = g_dCurTime;

			render();
		}
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;
	
    switch( msg )
	{	
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE: PostQuitMessage(0); break;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			if (g_GameState == MAIN && g_StartButton.isCollision(pt) == TRUE)
				g_GameState = LOADING;
			else if (g_GameState == MAIN && g_ExitButton.isCollision(pt) == TRUE)
				g_GameState = EXIT;

			ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
            ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
			bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			bMousing = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit.x = LOWORD (lParam);
			ptCurrentMousePosit.y = HIWORD (lParam);

			if( bMousing )
			{
				g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
				g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
			}
			
			ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
		}
		break;
		
		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	ChangeImg(&ptLastMousePosit);
	return 0;
}


//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DCAPS9 d3dCaps;
	g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

    D3DDISPLAYMODE d3ddm;
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
    d3dpp.Windowed               = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );

	
	g_pd3dDevice->CreateVertexBuffer( 4*sizeof(Vertex), D3DUSAGE_WRITEONLY, 
                                      D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, 
                                      &g_pVertexBuffer, NULL );
    void *pVertices = NULL;

    g_pVertexBuffer->Lock( 0, sizeof(g_quadVertices), (void**)&pVertices, 0 );
    memcpy( pVertices, g_quadVertices, sizeof(g_quadVertices) );
    g_pVertexBuffer->Unlock();

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
						(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CreateSprite();
}

void CreateSprite( void )
{
	// Main Menu Create
	g_MenuBackGround.Create(g_pd3dDevice, "Sprite/MainPage.bmp", 800, 600);
	g_StartButton.Create(g_pd3dDevice, "Sprite/Start_Button.bmp", 466, 95);
	g_ExitButton.Create(g_pd3dDevice, "Sprite/Exit_Button.bmp", 466, 95);

	// Loading Create
	g_Loading.Create(g_pd3dDevice, "Sprite/Loading.bmp", 1600, 1800);

	// InGame Create - BackGround
	g_GameBackGround.Create(g_pd3dDevice, "Sprite/InGameBackGround.bmp", 800, 600);
	g_SeaWave.Create(g_pd3dDevice, "Sprite/SeaWave.bmp", 800, 160);
	g_UserChar.Create(g_pd3dDevice, "Sprite/MyShip.bmp", 615, 320);
	
	g_SeaMonMgr.Create(g_pd3dDevice);
	g_AirMonMgr.Create(g_pd3dDevice);

	g_GameOver.Create(g_pd3dDevice, "Sprite/gameover.bmp", 400, 200);

	// Load Main Menu BGM
	g_sounds[0].LoadBGM("Sounds/6164__noisecollector__jillys-sonar.mp3");
	// Load In Game BGM
	g_sounds[1].LoadBGM("Sounds/Metal Slug 3 Music- Marine Diver (Mission One Routes Two & Three).mp3");
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
    if( g_pVertexBuffer != NULL ) 
        g_pVertexBuffer->Release(); 

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0 );

	g_pd3dDevice->BeginScene();


	if (g_GameState == MAIN)
	{
		// BGM - play
		g_sounds[0].RunBGM();
		g_sounds[0].OnEvent();

		// Layer 1 - Back Ground
		g_MenuBackGround.OnUpdate(g_fElpasedTime);

		// Layer 2 - Buttons
		g_StartButton.OnUpdate(g_fElpasedTime);
		g_ExitButton.OnUpdate(g_fElpasedTime);
	}
	else if (g_GameState == LOADING)
	{
		if (g_Loading.LoadCount == 3)
		{
			g_sounds[0].StopBGM();
			g_GameState = INGAME;
		}			

		// Loading Update
		g_Loading.OnUpdate(g_fElpasedTime);
	}
	else if (g_GameState == INGAME)
	{	
		// Game Exit
		if (g_UserChar.MyLife == 0)
		{
			g_ExitTime += g_fElpasedTime;

			// BGM 
			g_sounds[1].StopBGM();
			g_GameOver.pt_sound->RunBGM();

			// Layer 1 - Back Ground
			g_GameBackGround.OnDraw();	

			// Layer 2 - Sea Wave
			g_SeaWave.OnDraw();
			g_GameOver.OnDraw();

			if (g_ExitTime > 3.0f)
				g_GameState = EXIT;
		}					
		else
		{
			// BGM - play
			g_sounds[1].RunBGM();
			g_sounds[1].OnEvent();

			// Layer 1 - Back Ground
			g_GameBackGround.OnUpdate(g_fElpasedTime);

			// Layer 2 - Monster
			g_SeaMonMgr.OnUpdate(g_fElpasedTime, &g_UserChar);
			g_AirMonMgr.OnUpdate(g_fElpasedTime, &g_UserChar);

			// Layer 3 - User
			g_UserChar.OnUpdate(g_fElpasedTime);

			// Layer 4 - Sea Wave
			g_SeaWave.OnUpdate(g_fElpasedTime);
		}
	}
	else if (g_GameState == EXIT)
		exit(true);	

    g_pd3dDevice->EndScene();
	g_pd3dDevice->EndScene();

    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void ChangeImg(POINT* _ptLastMousePosit)
{
	if (g_GameState == MAIN)
	{
		if (g_StartButton.isCollision(*_ptLastMousePosit) == TRUE)
			g_StartButton.ChangeButtonImg(START_BUTTON_IMG::COL_START_ON);
		else if (g_StartButton.isCollision(*_ptLastMousePosit) == FALSE)
			g_StartButton.ChangeButtonImg(START_BUTTON_IMG::COL_START_OFF);

		if (g_ExitButton.isCollision(*_ptLastMousePosit) == TRUE)
			g_ExitButton.ChangeButtonImg(EXIT_BUTTON_IMG::COL_EXIT_ON);
		else if (g_ExitButton.isCollision(*_ptLastMousePosit) == FALSE)
			g_ExitButton.ChangeButtonImg(EXIT_BUTTON_IMG::COL_EXIT_OFF);
	}
}
