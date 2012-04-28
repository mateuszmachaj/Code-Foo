#include "PostGame.h"

// Get a reference to the DirectX Manager	
static Graphics& dxMgr = Graphics::getInstance();

// Get a reference to the DirectInput Manager	
static Input& diMgr = Input::getInstance();

// Get a reference to the DirectSound Manager
static Audio& dsMgr = Audio::getInstance();

PostGame::PostGame()
{
	selection = 0;
	win = NULL;
	lose = NULL;
	move = NULL;
	enter = true;
}
bool PostGame::Init()
{
	win = dsMgr.LoadSound("./Audio/win.wav");
	if (win == NULL)
        return false;

	lose = dsMgr.LoadSound("./Audio/lose.wav");
	if (lose == NULL)
        return false;
		
	move = dsMgr.LoadSound("./Audio/move.wav");
	if (move == NULL)
        return false;

	return true;
}

void PostGame::Render(int gameState)
{
	RECT rect;

	dxMgr.beginRender();
		
	rect.left = 250;
	rect.top = 150;
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 20;

	if(gameState == LOSE)
		dxMgr.drawFont("You Lose",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);
	else if(gameState == PLAYERONEWIN)
		dxMgr.drawFont("Player One Wins",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);
	else if(gameState == WIN)
		dxMgr.drawFont("You Win",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);
	else if(gameState == PLAYERTWOWIN)
		dxMgr.drawFont("Player Two Wins",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);
	else if(gameState == DRAW || gameState == MULTIDRAW)
		dxMgr.drawFont("Draw Game",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);
	else if(gameState == PAUSE || gameState == MULTIPAUSE)
		dxMgr.drawFont("Paused",&rect,D3DCOLOR_XRGB(0,255,0),DT_LEFT);

	rect.top += 20;
	rect.bottom = rect.top + 20;
	if(selection == 0)
		dxMgr.drawFont("Continue",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("Continue",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.top += 20;
	rect.bottom = rect.top + 20;
	if(selection == 1)
		dxMgr.drawFont("New Game",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("New Game",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.top += 20;
	rect.bottom = rect.top + 20;
	if(selection == 2)
		dxMgr.drawFont("Main Menu",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("Main Menu",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.top += 20;
	rect.bottom = rect.top + 20;
	if(selection == 3)
		dxMgr.drawFont("Quit",&rect,D3DCOLOR_XRGB(255,0,0),DT_LEFT);
	else
		dxMgr.drawFont("Quit",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	dxMgr.endRender();
}
int PostGame::Update(int gameState)
{
	diMgr.getKeyInput();

	if(enter)
	{
		enter = false;
		if(gameState == WIN || gameState == PLAYERONEWIN || gameState == PLAYERTWOWIN)
			dsMgr.PlaySound(win);
		else if(gameState == LOSE || gameState == DRAW || gameState == MULTIDRAW)
			dsMgr.PlaySound(lose);
	}

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
		else if(selection == 2)
			selection = 3;
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
		else if(selection == 3)
			selection = 2;
		dsMgr.PlaySound(move);
		diMgr.key[UP] = 0;
	}
		
	if((diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 0)
		diMgr.key[ACTION] = 1;
	else if((!diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 1)
		diMgr.key[ACTION] = 2;
	else if(diMgr.key[ACTION] == 2)
	{
		diMgr.key[ACTION] = 0;
		enter = true;
		if(selection == 0)
		{
			selection = 0;
			if(gameState == LOSE || gameState == WIN || gameState == DRAW)
				return CONTSINGLE;
			else if(gameState == PLAYERONEWIN || gameState == PLAYERTWOWIN || gameState == MULTIDRAW)
				return CONTMULTI;
			else if(gameState == PAUSE || gameState == MULTIPAUSE)
				return PLAY;
		}
		else if(selection == 1)
		{
			selection = 0;
			if(gameState == LOSE || gameState == WIN || gameState == DRAW || gameState == PAUSE)
				return SINGLEGAME;
			else if(gameState == PLAYERONEWIN || gameState == PLAYERTWOWIN || gameState == MULTIDRAW || gameState == MULTIPAUSE)
				return MULTIGAME;
		}
		else if(selection == 2)
		{
			selection = 0;
			return MAINMENU;
		}
		else
			PostQuitMessage(0);
	}
	
	return gameState;
}