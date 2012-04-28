#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

class Graphics
{
public:
	// singleton stuff
	static Graphics& getInstance() { static Graphics pInstance; return pInstance;}

	bool InitDirect3D(HWND hwnd);
	void ShutdownDirect3D();
	void beginRender();
	void drawSprite(D3DXVECTOR3* position, LPDIRECT3DTEXTURE9 texture, RECT* rect);
	void drawFont(char *text, RECT* rect, D3DCOLOR color, DWORD position);
	void endRender();
	LPDIRECT3DTEXTURE9 LoadTexture(char *filename, D3DCOLOR transcolor);

private:
	Graphics();

	LPDIRECT3D9 pD3D;
	LPDIRECT3DDEVICE9 pd3dDevice;
	LPD3DXSPRITE spriteHandler;
	ID3DXFont *font;
	
	// screen details
	int screen_width;
	int screen_height;
};