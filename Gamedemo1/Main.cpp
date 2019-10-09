#include <d3d9.h>
#include <Windows.h>
#include "Game.h"

#define WIDTH 320
#define HEIGHT 240


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstace,
	LPSTR lpStrCmdLine,
	int nShowCmd)
{
	Game mygame(WIDTH, HEIGHT, false);
	mygame.GameInit();
	mygame.GameRun();
	mygame.GameExit();
	return 0;
}