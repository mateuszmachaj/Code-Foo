#include "Game.h"
#include "GameStates.h"
#include <stdio.h>

// Get a reference to the DirectX Manager	
static Graphics& dxMgr = Graphics::getInstance();

// Get a reference to the DirectInput Manager	
static Input& diMgr = Input::getInstance();

// Get a reference to the DirectSound Manager
static Audio& dsMgr = Audio::getInstance();

Game::Game()
{
	redPiece = NULL;
	bluePiece = NULL;
	board = NULL;
	title = NULL;
	music = NULL;
	move = NULL;
	hit = NULL;
}

void Game::Shutdown()
{
	redPiece->Release();
	bluePiece->Release();
	board->Release();
	title->Release();
}

bool Game::InitGame()
{
	redPiece = dxMgr.LoadTexture("./Graphics/redpiece.bmp", D3DCOLOR_XRGB(255,0,255));
	if (redPiece == NULL)
        return false;

	bluePiece = dxMgr.LoadTexture("./Graphics/bluepiece.bmp", D3DCOLOR_XRGB(255,0,255));
	if (bluePiece == NULL)
        return false;

	board = dxMgr.LoadTexture("./Graphics/board.bmp", D3DCOLOR_XRGB(255,0,255));
	if (board == NULL)
        return false;
	
	title = dxMgr.LoadTexture("./Graphics/stitle.bmp", D3DCOLOR_XRGB(255,0,255));
	if (title == NULL)
        return false;

	music = dsMgr.LoadSound("./Audio/main.wav");
	if (music == NULL)
        return false;
	
	hit = dsMgr.LoadSound("./Audio/hit.wav");
	if (music == NULL)
        return false;
	
	move = dsMgr.LoadSound("./Audio/move.wav");
	if (music == NULL)
        return false;

	return true;
}

void Game::setUp(bool multiGame, bool newGame)
{
	selection = 0;
	pieceFall = false;
	humanOp = multiGame;

	if(newGame)
	{
		playerScore[0] = 0;
		playerScore[1] = 0;
		aiScore[0] = 0;
		aiScore[1] = 0;
		
		//first player is first
		playerTurn = 1;
	}

	//set up field
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			field[i][j] = 0;
		}
	}

	//set up pieces loc
	for(int i = 0; i < 7; i++)
	{
		playerPieces[i][0] = 5;
		playerPieces[i][1] = i;
		aiPieces[i][0] = 5;
		aiPieces[i][1] = i;
	}

	//set up piece defaults
	for(int i = 0; i < 7; i++)
	{
		for(int j = 2; j < 6; j++)
		{
			playerPieces[i][j] = 0;
			aiPieces[i][j] = 0;
		}
	}
			
    // Loop through and init red pieces
    for (int i = 0; i < 21; i++)
    {
        // Set the width and height of the sprite
        redPieces[i].width = 64.0f;
        redPieces[i].height = 64.0f;

        // Create and set a random x,y position
        redPieces[i].posX = 192;
        redPieces[i].posY = 0;

        // This sprite is visible
        redPieces[i].visible = false;
    }

	// Loop through and init blue pieces
    for (int i = 0; i < 21; i++)
    {
        // Set the width and height of the sprite
        bluePieces[i].width = 64.0f;
        bluePieces[i].height = 64.0f;

        // Create and set a random x,y position
        bluePieces[i].posX = 192;
        bluePieces[i].posY = 0;

        // This sprite is visible
        bluePieces[i].visible = false;
    }
	
	//set default number of pieces on the screen
	redCount = 0;
	blueCount = 0;

	//set first piece visible, if a continued game prev winner goes first
	if(playerTurn == 1)
		bluePieces[blueCount].visible = true;
	else
		redPieces[redCount].visible = true;
}

int Game::aiSeletion()
{
	int selection[2] = {-1};

	//check if any open places leads to a win
	for(int i = 0; i < 7; i++)
	{
		if(winCheck(aiPieces,i))
		{
			if(aiPieces[i][0] != -1)
				return i; 
		}
	}

	//check if player is close to a win
	for(int i = 0; i < 7; i++)
	{
		if(playerPieces[i][0] != -1)
		{
			for(int j = 2; j < 6; j++)
			{
				if(selection[1] < playerPieces[i][j])
				{
					selection[0] = i;
					selection[1] = playerPieces[i][j];
				}
			}
		}
	}

	//check if ai is closer to a win
	for(int i = 0; i < 7; i++)
	{
		if(aiPieces[i][0] != -1)
		{
			for(int j = 2; j < 6; j++)
			{
				if(selection[1] < aiPieces[i][j])
				{
					selection[0] = i;
					selection[1] = aiPieces[i][j];
				}
			}
		}
	}

	//if there is no other piece of the field comp chooses middle piece
	if(selection[0] == -1)
		selection[0] = 3;

	return selection[0];
}

bool Game::winCheck(int pieces[7][6], int input)
{
	if(pieces[input][2] + 1 > 3)
		return true;
	else if(pieces[input][3] + 1 > 3)
		return true;
	else if(pieces[input][4] + 1 > 3)
		return true;
	else if(pieces[input][5] + 1 > 3)
		return true;
	else
		return false;
}
bool Game::drawCheck()
{
	bool draw = true;
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			if(field[i][j] == 0)
				draw = false;
		}
	}
	if(draw)
		dsMgr.StopSound(music);

	return draw;
}

void Game::updateField(int field[6][7], int pieces[7][6], int piecesTurn)
{
	for(int i = 0; i < 7; i++)
	{
		//starting positions
		int y = pieces[i][0];
		int x = pieces[i][1];

		if(pieces[i][0] != -1)
		{
			//vertical count
			pieces[i][2] = 0;

			for(int j = 0; j < 3; j++)
			{
				if(y != 5)
				{
					y++;
					if(field[y][x] == piecesTurn)
						pieces[i][2]++;
					else
						break;
				}
			}

			//left horizontal count
			y = pieces[i][0];
			x = pieces[i][1];
			pieces[i][3] = 0;

			for(int j = 0; j < 3; j++)
			{
				if(x != 0)
				{
					x--;
					if(field[y][x] == piecesTurn)
						pieces[i][3]++;
					else
						break;
				}
			}

			//right horizontal count
			y = pieces[i][0];
			x = pieces[i][1];

			for(int j = 0; j < 3; j++)
			{
				if(x != 6)
				{
					x++;
					if(field[y][x] == piecesTurn)
						pieces[i][3]++;
					else
						break;
				}
			}

			//upper left diagonal count
			y = pieces[i][0];
			x = pieces[i][1];
			pieces[i][4] = 0;

			for(int j = 0; j < 3; j++)
			{
				if(x != 0 && y != 0)
				{
					x--;
					y--;
					if(field[y][x] == piecesTurn)
						pieces[i][4]++;
					else
						break;
				}
			}

			//lower left diagonal count
			y = pieces[i][0];
			x = pieces[i][1];

			for(int j = 0; j < 3; j++)
			{
				if(x != 6 && y != 5)
				{
					x++;
					y++;
					if(field[y][x] == piecesTurn)
						pieces[i][4]++;
					else
						break;
				}
			}

			//upper right diagonal count
			y = pieces[i][0];
			x = pieces[i][1];
			pieces[i][5] = 0;

			for(int j = 0; j < 3; j++)
			{
				if(x != 6 && y != 0)
				{
					x++;
					y--;
					if(field[y][x] == piecesTurn)
						pieces[i][5]++;
					else
						break;
				}
			}

			//lower right diagonal count
			y = pieces[i][0];
			x = pieces[i][1];

			for(int j = 0; j < 3; j++)
			{
				if(x != 0 && y != 5)
				{
					x--;
					y++;
					if(field[y][x] == piecesTurn)
						pieces[i][5]++;
					else
						break;
				}
			}
		}
	}
}

int Game::Update()
{
	//play music
	dsMgr.LoopSound(music);

	//check if game is a draw
	if(drawCheck())
	{
		if(humanOp)
			return MULTIDRAW;
		else
			return DRAW;
	}
	//update piece movement
	if(pieceFall == true)
	{
		if(playerTurn == 1)
		{
			bluePieces[blueCount].posY += 4;
			for(int i = 0; i < 7; i++)
			{
				if((playerPieces[i][0] + 1)*64 == bluePieces[blueCount].posY && playerPieces[i][1]*64 == bluePieces[blueCount].posX)
				{
					pieceFall = false;
					redPieces[redCount].visible = true;
					blueCount++;
					//winchecks
					if(winCheck(playerPieces,i))
					{
						playerScore[0]++;
						aiScore[1]++;
						dsMgr.StopSound(music);
						if(humanOp)
							return PLAYERONEWIN;
						else
							return WIN;
					}
											
					//update game data
					dsMgr.PlaySound(hit);
					field[playerPieces[i][0]][i] = 1;
					playerPieces[i][0]--;
					aiPieces[i][0]--;  
					updateField(field, playerPieces, 1);
					updateField(field, aiPieces, 2);
					playerTurn = 2;
				}
			}
		}
		else if(playerTurn == 2)
		{
			redPieces[redCount].posY += 4;
			for(int i = 0; i < 7; i++)
			{
				if((aiPieces[i][0] + 1)*64 == redPieces[redCount].posY && aiPieces[i][1]*64 == redPieces[redCount].posX)
				{
					pieceFall = false;
					bluePieces[blueCount].visible = true;
					redCount++;
					dsMgr.PlaySound(hit);
					//winchecks
					if(winCheck(aiPieces,i))
					{
						playerScore[1]++;
						aiScore[0]++;
						dsMgr.StopSound(music);
						if(humanOp)
							return PLAYERTWOWIN;
						else
							return LOSE;
					}
					
					//update game data
					field[aiPieces[i][0]][i] = 2;
					playerPieces[i][0]--;
					aiPieces[i][0]--;  
					updateField(field, playerPieces, 1);
					updateField(field, aiPieces, 2);
					playerTurn = 1;
				}
			}
		}
	}
	else
	{
		if(playerTurn == 1)
		{
			diMgr.getKeyInput();
			if((diMgr.isKeyDown(DIK_RIGHT)) && diMgr.key[RIGHT] == 0)
				diMgr.key[RIGHT] = 1;
			else if((!diMgr.isKeyDown(DIK_RIGHT)) && diMgr.key[RIGHT] == 1)
				diMgr.key[RIGHT] = 2;
			else if(diMgr.key[RIGHT] == 2)
			{
				if(bluePieces[blueCount].posX != playerPieces[6][1]*64)
				{
					bluePieces[blueCount].posX += 64;
					dsMgr.PlaySound(move);
				}
				diMgr.key[RIGHT] = 0;
			}

			if((diMgr.isKeyDown(DIK_LEFT)) && diMgr.key[LEFT] == 0)
				diMgr.key[LEFT] = 1;
			else if((!diMgr.isKeyDown(DIK_LEFT)) && diMgr.key[LEFT] == 1)
				diMgr.key[LEFT] = 2;
			else if(diMgr.key[LEFT] == 2)
			{
				if(bluePieces[blueCount].posX != playerPieces[0][1]*64)
				{
					bluePieces[blueCount].posX -= 64;
					dsMgr.PlaySound(move);
				}
				diMgr.key[LEFT] = 0;
			}

			if((diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 0)
				diMgr.key[ACTION] = 1;
			else if((!diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 1)
				diMgr.key[ACTION] = 2;
			else if(diMgr.key[ACTION] == 2)
			{
				bool pass = true;
				for(int i = 0; i < 7; i++)
				{
					if((playerPieces[i][0] + 1)*64 == bluePieces[blueCount].posY && playerPieces[i][1]*64 == bluePieces[blueCount].posX)
						pass = false;		
				}
				if(pass)
					pieceFall = true;
				diMgr.key[ACTION] = 0;
			}
		}
		else if(playerTurn == 2)
		{
			if(humanOp)
			{
				diMgr.getKeyInput();
				if((diMgr.isKeyDown(DIK_RIGHT)) && diMgr.key[RIGHT] == 0)
					diMgr.key[RIGHT] = 1;
				else if((!diMgr.isKeyDown(DIK_RIGHT)) && diMgr.key[RIGHT] == 1)
					diMgr.key[RIGHT] = 2;
				else if(diMgr.key[RIGHT] == 2)
				{
					if(redPieces[redCount].posX != playerPieces[6][1]*64)
					{
						redPieces[redCount].posX += 64;
						dsMgr.PlaySound(move);
					}
					diMgr.key[RIGHT] = 0;
				}

				if((diMgr.isKeyDown(DIK_LEFT)) && diMgr.key[LEFT] == 0)
					diMgr.key[LEFT] = 1;
				else if((!diMgr.isKeyDown(DIK_LEFT)) && diMgr.key[LEFT] == 1)
					diMgr.key[LEFT] = 2;
				else if(diMgr.key[LEFT] == 2)
				{
					if(redPieces[redCount].posX != playerPieces[0][1]*64)
					{
						redPieces[redCount].posX -= 64;
						dsMgr.PlaySound(move);
					}
					diMgr.key[LEFT] = 0;
				}

				if((diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 0)
					diMgr.key[ACTION] = 1;
				else if((!diMgr.isKeyDown(DIK_SPACE)) && diMgr.key[ACTION] == 1)
					diMgr.key[ACTION] = 2;
				else if(diMgr.key[ACTION] == 2)
				{
					bool pass = true;
					for(int i = 0; i < 7; i++)
					{
						if((aiPieces[i][0] + 1)*64 == redPieces[redCount].posY && aiPieces[i][1]*64 == redPieces[redCount].posX)
							pass = false;		
					}
					if(pass)
						pieceFall = true;
					diMgr.key[ACTION] = 0;
				}
			}
			else
			{
				redPieces[redCount].posX = (float)aiSeletion()*64;
				pieceFall = true;
			}
		}
		//check for pause input
		if((diMgr.isKeyDown(DIK_RETURN)) && diMgr.key[UP] == 0)
			diMgr.key[UP] = 1;
		else if((!diMgr.isKeyDown(DIK_RETURN)) && diMgr.key[UP] == 1)
			diMgr.key[UP] = 2;
		else if(diMgr.key[UP] == 2)
		{
			diMgr.key[UP] = 0;
			dsMgr.StopSound(music);
			if(humanOp)
				return MULTIPAUSE;
			else
				return PAUSE;
		}
	}
	return PLAY;
}
void Game::Render()
{
	dxMgr.beginRender();

	RECT rect;
	//draw all visible pieces
	for(int i = 0; i < 21; i++)
	{
		if(redPieces[i].visible == true)
			dxMgr.drawSprite(&D3DXVECTOR3(redPieces[i].posX,redPieces[i].posY,0), redPiece, NULL);

		if(bluePieces[i].visible == true)
			dxMgr.drawSprite(&D3DXVECTOR3(bluePieces[i].posX,bluePieces[i].posY,0), bluePiece, NULL);
	}

	//draw the board over the pieces
	dxMgr.drawSprite(&D3DXVECTOR3(0,64,0), board, NULL);

	//draw logo
	dxMgr.drawSprite(&D3DXVECTOR3(460,250,0), title, NULL);

	//draws game data on the screen mainly wins and loses
	rect.left = 475;
	rect.top = 10;
	rect.right= rect.left + 150;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Scores:",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);

	rect.left = 475;
	rect.top += 30;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Player 1:",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);
	
	rect.left = 440;
	rect.top += 30;
	rect.right= rect.left + 100;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Wins:",&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);
	
	//int to LPCWSTR conversion variables
	char score [3];

	sprintf_s(score, "%d", playerScore[0]);
	
	rect.left = 475;
	rect.right= rect.left + 100;
	dxMgr.drawFont(score,&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);

	rect.left = 440;
	rect.top += 30;
	rect.right= rect.left + 100;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Loses:",&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);
	
	sprintf_s(score, "%d", playerScore[1]);

	rect.left = 475;
	rect.right= rect.left + 100;
	dxMgr.drawFont(score,&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);

	rect.left = 475;
	rect.top += 30;
	rect.right= rect.left + 100;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Player 2:",&rect,D3DCOLOR_XRGB(255,255,255),DT_LEFT);
	
	rect.left = 440;
	rect.top += 30;
	rect.right= rect.left + 100;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Wins:",&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);
	
	sprintf_s(score, "%d", aiScore[0]);

	rect.left = 475;
	rect.right= rect.left + 100;
	dxMgr.drawFont(score,&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);

	rect.left = 440;
	rect.top += 30;
	rect.right= rect.left + 100;
	rect.bottom = rect.top + 30;
	dxMgr.drawFont("Loses:",&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);
	
	sprintf_s(score, "%d", aiScore[1]);

	rect.left = 475;
	rect.right= rect.left + 100;
	dxMgr.drawFont(score,&rect,D3DCOLOR_XRGB(255,255,255),DT_RIGHT);

	dxMgr.endRender();
}