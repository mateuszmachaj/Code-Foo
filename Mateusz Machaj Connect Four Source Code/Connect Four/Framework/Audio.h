#pragma once
#include "dsutil.h"

class Audio
{
public:
	static Audio& getInstance() { static Audio pInstance; return pInstance;}

	bool init(HWND hwnd);
	CSound *LoadSound(char *filename);
	void PlaySound(CSound *sound);
	void LoopSound(CSound *sound);
	void StopSound(CSound *sound);

private:
	
	CSoundManager *dsound;
};