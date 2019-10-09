#include "Game.h"

#include <d3dx9.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

#define WINDOW_CLASS_NAME L"Game"
#define TITLE L"GAME"
#define BRICK_TEXTURE_PATH L"mario.png"
#define MAX_FRAME_RATE 15;
#define BACKGROUND_COLOR D3DCOLOR_XRGB(100, 100, 100)

int brick_x = 100;
int brick_y = 100;
float v = 0.1f;

LPDIRECT3DSURFACE9 backBuffer = NULL;
LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

LPDIRECT3DTEXTURE9 texBrick;				// texture object to store brick image

void DebugOut(wchar_t* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

Game::Game(int scr_Width, int scr_Height, bool isWindow) {
	this->m_Width = scr_Width;
	this->m_Height = scr_Height;
	this->m_IsWindow = isWindow;
	this->m_HandleInstance = 0;
	this->m_HandleWindow = 0;
	this->m_lpDirect3D = 0;
	this->m_lpDirect3DDevice = 0;
}

void Game::initHandleWindow()
{
	WNDCLASSEX WndcEx;
	ZeroMemory(&WndcEx, sizeof(WndcEx));
	WndcEx.cbSize = sizeof(WNDCLASSEX);
	WndcEx.hInstance = this->m_HandleInstance;
	WndcEx.lpfnWndProc = (WNDPROC)WndProceduce;
	WndcEx.lpszClassName = WINDOW_CLASS_NAME;
	if (!RegisterClassEx(&WndcEx))
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return;
	}
	this->m_HandleWindow = CreateWindow(
		WINDOW_CLASS_NAME, // đặt trung với WndcEx.lpszClassName
		TITLE, // TITLE của cửa sổ #define TITLE "GAME"
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // style của cửa sổ
		100, // tọa độ x khi cửa sổ xuất hiện
		100, // tọa độ y khi cửa sổ xuất hiện
		this->m_Width, // chiều dài cửa sổ
		this->m_Height, // chiều cao cửa sổ
		0, // handle window parent của của sổ hiện tại. Để là 0
		0, // handle đến Menu. Để là 0
		this->m_HandleInstance, // Handle Instance của ứng dụng
		0); // giữ nguyên theo default là 0
	if (!this->m_HandleWindow) // Kiểm tra tạo thành công.
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return;
	}
	ShowWindow(m_HandleWindow, SW_SHOW); // show cửa sổ lên màn hình
	UpdateWindow(m_HandleWindow); // Update cửa sổ theo thời gian
}

void Game::initDirectX()
{
	this->m_lpDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_lpDirect3D) {
		D3DPRESENT_PARAMETERS d3dpp; // khai báo biến
		ZeroMemory(&d3dpp, sizeof(d3dpp)); // Khởi tạo vùng nhớ
		d3dpp.BackBufferCount = 1; // Số lượng Back buffer
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Chọn UNKNOW, DirectX sẽ tự chọn 1 định dạng phù hợp cho Device				
		d3dpp.BackBufferHeight = m_Height + 1; // Chiều cao BackBuffet
		d3dpp.BackBufferWidth = m_Width + 1; //Chiều dài
		d3dpp.Windowed = true; // Window ở dạng cửa sổ, không phải full màn hình
		d3dpp.hDeviceWindow = this->m_HandleWindow; // Gán HandleWindow cho Device
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Phương thức swap của Buffer
		
		m_lpDirect3D->CreateDevice(
			D3DADAPTER_DEFAULT, // Chọn DEFAULT ADAPTER
			D3DDEVTYPE_HAL, // 
			m_HandleWindow, // Handle Window của của sổ
			D3DCREATE_HARDWARE_VERTEXPROCESSING, // Hard ware vertex processing
			&d3dpp,
			&m_lpDirect3DDevice); // Hàm tạo Device
		
		if (FAILED(m_lpDirect3DDevice))
		{
			OutputDebugString(L"[ERROR] CreateDevice failed\n");
			return;
		}		
		m_lpDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
		D3DXCreateSprite(m_lpDirect3DDevice, &spriteHandler);	
		OutputDebugString(L"[INFO] InitGame is done\n");
	}	
}

void Game::GameInit()
{
	initHandleWindow();	
	initDirectX();
	LoadResources();
}

void Game::GameRun()
{
	MSG msg;
	DWORD frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	ZeroMemory(&msg, sizeof(msg)); // Tạo một Mesage để lắng nghe các sự kiện của cửa sổ
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // Lấy message và truyền vào biến msg đã khai báo
		{
			if (msg.message == WM_QUIT) // Kiểm tra nếu msg yêu cầu QUIT ứng dụng thì ứng dụng sẽ thoát
				break;
			TranslateMessage(&msg); // xử lý input và chuyển vào msg
			DispatchMessage(&msg); // gửi message đến hàm WndProceduce để xử lý
		}
		DWORD now = GetTickCount64();
		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);		
	}
}

void Game::GameExit()
{
	if (this->m_lpDirect3D) {
		this->m_lpDirect3D->Release();
		this->m_lpDirect3D = NULL;
	}
	if (this->m_lpDirect3DDevice) {
		this->m_lpDirect3DDevice->Release();
		this->m_lpDirect3DDevice = NULL;
	}
}

void Game::Update(DWORD dt) {
	brick_x += v * dt;
	if (brick_x >= m_Width - 16 || brick_x <= 0) {
		v *= -1;
	}
}

void Game::Render() {	
		//Draw something in Game
		m_lpDirect3DDevice->Clear(0, 0, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0f, 0); // Clear cửa sổ 1 lần trước khi vẽ lên
		if (m_lpDirect3DDevice->BeginScene())
		{
			//m_lpDirect3DDevice->ColorFill(backBuffer, NULL, BACKGROUND_COLOR);

			spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);// Game Render Begin Here

			D3DXVECTOR3 p((float)brick_x, (float)brick_y, 0);
			spriteHandler->Draw(texBrick, NULL, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));

			spriteHandler->End();
			
			m_lpDirect3DDevice->EndScene();
		}
		m_lpDirect3DDevice->Present(0, 0, 0, 0); // Thể hiện tất cả những gì đã vẽ lên màn hình
}

void Game::LoadResources()
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(BRICK_TEXTURE_PATH, &info);
	if (result != D3D_OK)
	{		
		//DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", BRICK_TEXTURE_PATH);
		return;
	}

	result = D3DXCreateTextureFromFileEx(
		m_lpDirect3DDevice,								// Pointer to Direct3D device object
		BRICK_TEXTURE_PATH,					// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255),			// Transparent color
		&info,
		NULL,
		&texBrick);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}	
	//DebugOut(L"[INFO] Texture loaded Ok: %s \n", BRICK_TEXTURE_PATH);
}

LRESULT CALLBACK Game::WndProceduce(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
