#pragma once
//#ifndef __GAME_H__
#define __GAME_H__

#include <d3d9.h>

class Game
{
private:
	int					m_Width;
	int					m_Height;
	bool				m_IsWindow;

	HINSTANCE			m_HandleInstance;// đại diện cho ứng dụng khi hệ điệu hành chạy
	HWND				m_HandleWindow;// Thể hiện định danh khi có nhiều cửa sổ được mở
	LPDIRECT3D9			m_lpDirect3D;// Khởi tạo device để vẽ 
	LPDIRECT3DDEVICE9 	m_lpDirect3DDevice;// để lưu thông tin của Device sử dụng thao tác vẽ trong chương trình

	static LRESULT CALLBACK WndProceduce(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);// hàm callback dùng để lắng nghe các sự kiện của cửa sổ Game khi người dùng tương tác

public:
	Game(int scr_Width, int scr_Height, bool isWindow);// Hàm tạo của GameObject

	void initHandleWindow();// Khởi tạo các thuộc tính cần thiết cho cửa sổ
	void initDirectX();// Khởi tạo thiết bị DirectX

	void GameInit();
	void GameRun();
	void GameExit();
	void Update(DWORD dt);
	void Render();
	void LoadResources();
};
//#endif


