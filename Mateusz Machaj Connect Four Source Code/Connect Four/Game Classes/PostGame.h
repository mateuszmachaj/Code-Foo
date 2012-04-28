#include "../Framework/Graphics.h"
#include "../Framework/Input.h"
#include "../Framework/Audio.h"
#include "GameStates.h"

class PostGame
{
public:
	PostGame();
	void Render(int gameState);
	int Update(int gameState);
	bool Init();

private:
	int selection;
	CSound *win;
	CSound *lose;
	CSound *move;
	bool enter;
};