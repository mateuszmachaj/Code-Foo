#include <windows.h>
#include <tchar.h>
#include "./Framework/Graphics.h"
#include "./Framework/Input.h"
#include "./Framework/Audio.h"
#include "./Game Classes/GameStates.h"
#include "./Game Classes/Game.h"
#include "./Game Classes/MainMenu.h"
#include "./Game Classes/PostGame.h"

// Globals
HINSTANCE hInst;
HWND mainhWnd;
int gameState = MAINMENU;

//Game classes
Game playMode;
MainMenu mainMenu;
PostGame postGame;

// Forward declarations
bool InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Get a reference to the DirectX Manager	
static Graphics& dxMgr = Graphics::getInstance();

// Get a reference to the DirectInput Manager	
static Input& diMgr = Input::getInstance();

// Get a reference to the DirectSound Manager
static Audio& dsMgr = Audio::getInstance();

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// save off the instance handle
	hInst = hInstance; 

	// Set up the application window
	if (!InitWindow(hInstance))
	{
		return 0;
	}

	// Initialize Direct3D
	if (!dxMgr.InitDirect3D(mainhWnd))
	{
		return 0;
	}

	if (!diMgr.initDirectInput(hInst, mainhWnd))
	{
		MessageBox(mainhWnd, "Unable to init DirectInput", "ERROR", MB_OK);
		return false;
	}
	
	if (!diMgr.initKeyboard(hInst, mainhWnd))
	{
		MessageBox(mainhWnd, "Unable to init Mouse", "ERROR", MB_OK);
		return false;
	}

	if (!dsMgr.init(mainhWnd))
	{
		MessageBox(mainhWnd, "Unable to init Audio", "ERROR", MB_OK);
		return false;
	}

	// Initialize play mode
	if (!playMode.InitGame())
	{
		MessageBox(mainhWnd, "Unable to init game mode.", "ERROR", MB_OK);
		return 0;
	}

	
	// Initialize post game mode
	if (!postGame.Init())
	{
		MessageBox(mainhWnd, "Unable to init post game mode.", "ERROR", MB_OK);
		return 0;
	}

	// Initialize Main Menu
	if (!mainMenu.InitMenu())
	{
		MessageBox(mainhWnd, "Unable to init main menu.", "ERROR", MB_OK);
		return 0;
	}

	// Main message loop
    MSG msg = {0};
    while (WM_QUIT != msg.message)
    {
		// Process Windows messages first
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        	
		// Render the scene
		if(gameState == MAINMENU)
		{
			gameState = mainMenu.Update();
			mainMenu.Render();
		}
		else if(gameState == SINGLEGAME)
		{
			playMode.setUp(false,true);
			gameState = PLAY;
		}
		else if(gameState == MULTIGAME)
		{
			playMode.setUp(true,true);
			gameState = PLAY;
		}
		else if(gameState == CONTSINGLE)
		{
			playMode.setUp(false,false);
			gameState = PLAY;
		}
		else if(gameState == CONTMULTI)
		{
			playMode.setUp(true,false);
			gameState = PLAY;
		}
		else if(gameState == PLAYERONEWIN || gameState == PLAYERTWOWIN || gameState == LOSE || gameState == WIN || gameState == MULTIDRAW || gameState == DRAW || gameState == MULTIPAUSE || gameState == PAUSE)
		{
			gameState = postGame.Update(gameState);
			postGame.Render(gameState);
		}
		else if(gameState == PLAY)
		{
			gameState = playMode.Update();
			playMode.Render();
		}
    }

	// Clean up
	dxMgr.ShutdownDirect3D();
	diMgr.shutdown();
	playMode.Shutdown();
	mainMenu.Shutdown();

	return (int) msg.wParam;
}

bool InitWindow(HINSTANCE hInstance)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(NULL, IDI_INFORMATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = "Connect Four";
    wcex.hIconSm        = LoadIcon(NULL, IDI_INFORMATION);
    if(!RegisterClassEx(&wcex))
	{
        return false;
	}
	
	// create the window from the class above
    mainhWnd = CreateWindow("Connect Four", 
							"Connect Four", 
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							640, 
							480, 
							NULL, 
							NULL, 
							hInstance, 
							NULL);
    if(!mainhWnd)
	{
        return false;
	}

    ShowWindow(mainhWnd, SW_SHOW);
	UpdateWindow(mainhWnd);
   
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
		// Allow the user to press the escape key to end the application
        case WM_KEYDOWN:
			switch(wParam)
			{
				// Check if the user hit the escape key
				case VK_ESCAPE:
					//PostQuitMessage(0);
				break;
			}
        break;

		// The user hit the close button, close the application
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
