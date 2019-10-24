#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "GameScene.h"
#include "debug.h"
#include "Game.h"
#include "Textures.h"
#include "Keyboard.h"
#include "SIMON.h"
#include "Brick.h"
#include "Camera.h"
#include "MorningStar.h"
#include "Torch.h"

#include<fstream>
#include<iostream>
#include<fstream>
#include<string>
#include<queue>

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_TORCH 30

#define ID_TEX_ENTRANCESTAGE 100

#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 230

#define Scene_Intro 0
#define Scene_Stage1 1

class GameScene
{
private:
	static class GameScene* _Instance;
	int level;
public :
	GameScene() { 
	level = Scene_Intro; 
	}

	static GameScene* GetInstance();
    void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void LoadResources();
	void Update(DWORD dt);
	void Render();


	virtual ~GameScene() { }
};

