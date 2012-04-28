#pragma once

#include "../Framework/Graphics.h"
#include "../Framework/Input.h"
#include "../Framework/Audio.h"

// Sprite structure
typedef struct
{
    // sprite details
    float width;
    float height;

    // sprite position
    float posX;
    float posY;

    bool visible;

}Sprite;

class Game
{
public:
	Game();
	int Update();
	void Render();
	void setUp(bool multiGame, bool newGame);
	bool InitGame();
	void Shutdown();

private:
	int field[6][7];
	int playerPieces[7][6];
	int aiPieces[7][6];
	int playerTurn;
	int selection;
	Sprite redPieces[21];
	Sprite bluePieces[21];
	LPDIRECT3DTEXTURE9 redPiece;
	LPDIRECT3DTEXTURE9 bluePiece;
	LPDIRECT3DTEXTURE9 board;
	LPDIRECT3DTEXTURE9 title;
	int redCount;
	int blueCount;
	bool pieceFall;
	bool humanOp;
	int playerScore[2];
	int aiScore[2];
	CSound *music;
	CSound *move;
	CSound *hit;

	int aiSeletion();
	bool winCheck(int pieces[7][6], int input);
	void updateField(int field[6][7], int pieces[7][6], int playerTurn);
	bool drawCheck();
};