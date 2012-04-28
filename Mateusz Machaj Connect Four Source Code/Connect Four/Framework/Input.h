#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define BUTTONDOWN(name, key) (name.rgbButtons[key] & 0x80) 
#define KEYDOWN(name, key) (name[key] & 0x80)

class Input
{
public:

	static Input& getInstance() { static Input pInstance; return pInstance;}

	bool initDirectInput(HINSTANCE hInst, HWND wndHandle);
	bool initMouse(HINSTANCE hInst, HWND wndHandle);
	bool initKeyboard(HINSTANCE hInst, HWND wndHandle);

	void shutdown();

	void getInput();
	void getKeyInput();
	int key[5];

	bool isButtonDown(int which);
	bool isKeyDown(int which);

	inline int getCurMouseX() { return mouseState.lX; }
	inline int getCurMouseY() { return mouseState.lY; }
private:
	Input();

	LPDIRECTINPUT8        g_lpDI;
	LPDIRECTINPUTDEVICE8  g_lpDIDevice; 
	LPDIRECTINPUTDEVICE8  g_lpDIDeviceKey; 

	DIMOUSESTATE mouseState;			
	char buffer[256];				

}; 