#include "MainMenu.h"

// Get a reference to the DirectX Manager	
static Graphics& dxMgr = Graphics::getInstance();

// Get a reference to the DirectInput Manager	
static Input& diMgr = Input::getInstance();

// Get a reference to the DirectSound Manager
static Audio& dsMgr = Audio::getInstance();

MainMenu::MainMenu()
{
	selection = 0;
	logo = NULL;
	intro = NULL;
	move = NULL;
}
void MainMenu::Shutdown()
{
	logo->Release();
}
bool MainMenu::InitMenu()
{
	logo = dxMgr.LoadTexture("./Graphics/title.bmp", D3DCOLOR_XRGB(255,0,255));
	if (logo == NULL)
        return false;

	intro = dsMgr.LoadSound("./Audio/intro.wav");
	if(intro == NULL)
		return false;
		
	move = dsMgr.LoadSound("./Audio/move.wav");
	if (move == NULL)
        return false;

	return true;
}
void MainMenu::Render()
{
	RECT rect;

	dxMgr.beginRender();

	//logo draw
	dxMgr.drawSprite(&D3DXVECTOR3(160,75,0), logo, NULL);
		
	rect.left = 220;
	rect.top = 290;
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 30;

	if(selection == 0)
		dxMgr.drawFont("1 Player Game",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("1 Player Game",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.top += 30;
	rect.bottom = rect.top + 30;
	if(selection == 1)
		dxMgr.drawFont("2 Player Game",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("2 Player Game",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.top += 30;
	rect.bottom = rect.top + 30;
	if(selection == 2)
		dxMgr.drawFont("Quit",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("Quit",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	dxMgr.endRender();
}
int MainMenu::Update()
{
	//play music
	dsMgr.LoopSound(intro);

	diMgr.getKeyInput();

	if((diMgr.isKeyDown(DIK_DOWN)) && diMgr.key[DOWN] == 0)
		diMgr.key[DOWN] = 1;
	else if((!diMgr.isKeyDown(DIK_DOWN)) && diMgr.key[DOWN] == 1)
		diMgr.key[DOWN] = 2;
	else if(diMgr.key[DOWN] == 2)
	{
		if(selection == 0)
			selection = 1;
		else if(selection == 1)
			selection = 2;
		dsMgr.PlaySound(move);
		diMgr.key[DOWN] = 0;
	}

	if((diMgr.isKeyDown(DIK_UP)) && diMgr.key[UP] == 0)
		diMgr.key[UP] = 1;
	else if((!diMgr.isKeyDown(DIK_UP)) && diMgr.key[UP] == 1)
		diMgr.key[UP] = 2;
	else if(diMgr.key[UP] == 2)
	{
		if(selection == 1)
			selection = 0;
		else if(selection == 2)
			selection = 1;
		dsMgr.PlaySound(move);
		diMgr.key[UP] = 0;
	}
		
	if((diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 0)
		diMgr.key[ACTION] = 1;
	else if((!diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 1)
		diMgr.key[ACTION] = 2;
	else if(diMgr.key[ACTION] == 2)
	{
		dsMgr.StopSound(intro);
		diMgr.key[ACTION] = 0;
		if(selection == 0)
		{
			selection = 0;
			return SINGLEGAME;
		}
		else if(selection == 1)
		{
			selection = 0;
			return MULTIGAME;
		}
		else
			PostQuitMessage(0);
	}

	return MAINMENU;
}