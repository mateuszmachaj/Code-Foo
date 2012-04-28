#include "../Framework/Graphics.h"
#include "../Framework/Input.h"
#include "../Framework/Audio.h"
#include "GameStates.h"

class MainMenu
{
public:
	MainMenu();
	bool InitMenu();
	void Render();
	int Update();
	void Shutdown();

private:
	int selection;
	LPDIRECT3DTEXTURE9 logo;
	CSound *intro;
	CSound *move;
};