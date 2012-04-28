#include "Input.h"

Input::Input()
{
	g_lpDI = NULL;
	g_lpDIDevice = NULL;
}

bool Input::initDirectInput(HINSTANCE hInst, HWND wndHandle)
{
	HRESULT hr;

	// Create the DirectInput object. 
    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
                            IID_IDirectInput8, (void**)&g_lpDI, NULL); 

	if FAILED(hr) 
		return FALSE; 

    // Retrieve a pointer to an IDirectInputDevice8 interface 
    hr = g_lpDI->CreateDevice(GUID_SysMouse, &g_lpDIDevice, NULL); 
	if FAILED(hr)
		return FALSE;  

	// Retrieve a pointer to an IDirectInputDevice8 interface 
    hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDeviceKey, NULL); 

	//set up default values for key presses
	for(int i = 0; i < 5; i++)
	{
		key[i] = 0;
	}

	return true;
}
bool Input::initMouse(HINSTANCE hInst, HWND wndHandle)
{
	HRESULT hr;
	
	hr = g_lpDIDevice->SetDataFormat(&c_dfDIMouse); 
	if FAILED(hr)
		return FALSE; 
	
	// Set the cooperative level 
    hr = g_lpDIDevice->SetCooperativeLevel(wndHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE); 
    if FAILED(hr) 
        return FALSE; 

    // Get access to the input device. 
    hr = g_lpDIDevice->Acquire(); 
    if FAILED(hr) 
        return FALSE; 

	return true;
}

bool Input::initKeyboard(HINSTANCE hInst, HWND wndHandle)
{
	HRESULT hr;
	
	// Init the keyboard
	hr = g_lpDIDeviceKey->SetDataFormat(&c_dfDIKeyboard); 

	if FAILED(hr)
	{ 
		return FALSE; 
	} 

	// Set the cooperative level 
    hr = g_lpDIDeviceKey->SetCooperativeLevel(wndHandle, 
                             DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
    if FAILED(hr) 
    { 
        return FALSE; 
    } 

    // Get access to the input device. 
    hr = g_lpDIDeviceKey->Acquire(); 
    if FAILED(hr) 
    { 
        return FALSE; 
    }

	return true;
}

void Input::shutdown()
{
	if (g_lpDI) 
    { 
        if (g_lpDIDevice) 
        { 
			// Always unacquire device before calling Release(). 
            g_lpDIDevice->Unacquire(); 
            g_lpDIDevice->Release();
            g_lpDIDevice = NULL; 
        } 
        g_lpDI->Release();
        g_lpDI = NULL; 
    } 
}

void Input::getInput()
{
	HRESULT hr;

	hr = g_lpDIDevice->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mouseState); 

	// check the return state to see if the device is still accessible
	if (FAILED ( hr ))
	{
		// try and reacquire the input device
		hr = g_lpDIDevice->Acquire();
		// do a continuous loop until the device is reacquired
	       while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIDevice->Acquire();
	}			
}
void Input::getKeyInput()
{
	HRESULT hr;

	hr = g_lpDIDeviceKey->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 

	// check the return state to see if the device is still accessible
	if (FAILED ( hr ))
	{
		// try and reacquire the input device
		hr = g_lpDIDeviceKey->Acquire();
		// do a continuous loop until the device is reacquired
	       while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIDeviceKey->Acquire();
	}			
}

bool Input::isButtonDown(int which)
{
	return (bool)BUTTONDOWN(mouseState, which);
}

bool Input::isKeyDown(int which)
{
	return (bool)KEYDOWN(buffer, which);
}