#include "Graphics.h"

Graphics::Graphics()
{
	pD3D = NULL;
 	pd3dDevice = NULL;
	spriteHandler = NULL;

	screen_width = 640;
	screen_height = 480;
}

bool Graphics::InitDirect3D(HWND hwnd)
{
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = screen_height;
	d3dpp.BackBufferWidth  = screen_width;
	d3dpp.hDeviceWindow    = hwnd;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.EnableAutoDepthStencil = TRUE;

    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
        return false;
    }

	if(FAILED(D3DXCreateSprite(pd3dDevice, &spriteHandler)))
	{
		return false;
	}

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,TRUE );

	D3DXCreateFont( pd3dDevice, 22, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font );
	
	return true;
}

void Graphics::ShutdownDirect3D()
{
    if( pd3dDevice != NULL) 
	{
        pd3dDevice->Release();
		pd3dDevice = NULL;
	}
    if( pD3D != NULL)
	{
        pD3D->Release();
		pD3D = NULL;
	}

	if( spriteHandler != NULL)
	{
        spriteHandler->Release();
		spriteHandler = NULL;
	}
}

void Graphics::beginRender()
{
	if( NULL == pd3dDevice )
        return;

    // Clear the backbuffer to a black color
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	pd3dDevice->BeginScene();

	//start sprite handler
    spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::endRender()
{	
	//stop drawing
    spriteHandler->End();

	pd3dDevice->EndScene();

	// Present the backbuffer contents to the display
    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
void Graphics::drawSprite(D3DXVECTOR3* position, LPDIRECT3DTEXTURE9 texture, RECT* rect)
{
	spriteHandler->Draw(texture,rect,NULL,position,D3DCOLOR_XRGB(255,255,255));
}

LPDIRECT3DTEXTURE9 Graphics::LoadTexture(char *filename, D3DCOLOR transcolor)
{
    //the texture pointer
    LPDIRECT3DTEXTURE9 texture = NULL;

    //the struct for reading bitmap file info
    D3DXIMAGE_INFO info;

    //standard Windows return value
    HRESULT result;
    
    //get width and height from bitmap file
    result = D3DXGetImageInfoFromFile(filename, &info);
    if (result != D3D_OK)
        return NULL;

    //create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
        pd3dDevice,              //Direct3D device object
        filename,            //bitmap filename
        info.Width,          //bitmap image width
        info.Height,         //bitmap image height
        1,                   //mip-map levels (1 for no chain)
        D3DPOOL_DEFAULT,     //the type of surface (standard)
        D3DFMT_UNKNOWN,      //surface format (default)
        D3DPOOL_DEFAULT,     //memory class for the texture
        D3DX_DEFAULT,        //image filter
        D3DX_DEFAULT,        //mip filter
        transcolor,          //color key for transparency
        &info,               //bitmap file info (from loaded file)
        NULL,                //color palette
        &texture );          //destination texture

    //make sure the bitmap textre was loaded correctly
    if (result != D3D_OK)
        return NULL;

    return texture;
}

void Graphics::drawFont(char *text, RECT* rect, D3DCOLOR color, DWORD position)
{
	font->DrawText(NULL, text, -1, rect, position, color);
}