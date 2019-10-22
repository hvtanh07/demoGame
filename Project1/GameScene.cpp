#include "GameScene.h"



CSIMON* SIMON;
vector<LPGAMEOBJECT> objects;

GameScene* GameScene::_Instance = NULL;
GameScene* GameScene::GetInstance()
{
	if (_Instance == NULL)
		_Instance = new GameScene();
	return _Instance;
}

void GameScene::KeyState(BYTE* state)
{
	
	// disable control key when SIMON die 
		if (SIMON->GetState() == SIMON_STATE_DIE) return;
	if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		SIMON->SetState(SIMON_RIGHT);				
	}
	else if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
	{
		SIMON->SetState(SIMON_LEFT);
	}
	else if (CGame::GetInstance()->IsKeyDown(DIK_DOWN))
	{
		SIMON->SetState(SIMON_STATE_SIT);
	}
	else
		SIMON->SetState(SIMON_STATE_IDLE);
}

void GameScene::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		SIMON->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: // reset
		SIMON->SetState(SIMON_STATE_ATTACK);
		break;
	case DIK_R: // reset
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Setbboxrender();
		break;
	}
}

void GameScene::OnKeyUp(int KeyCode)
{
	if (KeyCode == 208) {
		SIMON->Standup();
	}
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void GameScene::LoadResources()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	textures->Add(ID_TEX_SIMON, L"textures\\Simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MISC, L"textures\\brick1.png", D3DCOLOR_XRGB(0, 0, 0));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_ENTRANCESTAGE, L"textures\\entrance.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 texEntrance = textures->Get(ID_TEX_ENTRANCESTAGE);

	///left top right bottom
	int l = 0, r = 32;
	for (int i = 1; i <= 20; i++)
	{
		sprites->Add(i, l, 0, r, 32, texEntrance);
		l += 32;
		r += 32;
	}
	l = 0;
	r = 32;
	for (int i = 21; i <= 38; i++)
	{
		sprites->Add(i, l, 32, r, 64, texEntrance);
		l += 32;
		r += 32;
	}

	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	LPDIRECT3DTEXTURE9 texSIMON = textures->Get(ID_TEX_SIMON);
	vector<int> numbers;

	int flag = 0;
	int number;
	int arr[5];
	ifstream file_simon("Simon.txt");
	if (file_simon.is_open())
	{
		while (!file_simon.eof())
		{
			while (file_simon >> number)
			{
				arr[flag] = number;
				flag++;
				if (flag == 5)
				{
					sprites->Add(arr[0], arr[1], arr[2], arr[3], arr[4], texSIMON);
					flag = 0;
				}
			}
		}
	}

	sprites->Add(10099, 215, 120, 231, 135, texSIMON);		// die 

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(90001, 0, 0, 32, 32, texMisc);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);

	LPANIMATION ani;

	ani = new CAnimation(100);	// idle right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle left
	ani->Add(10011);
	animations->Add(401, ani);


	ani = new CAnimation(100);	// walk right
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);
	ani = new CAnimation(100);	//  walk left
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// attack right
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// attack left
	ani->Add(10014);
	ani->Add(10015);
	ani->Add(10016);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// jump right
	ani->Add(10008);
	animations->Add(403, ani);
	ani = new CAnimation(100);	// jump right
	ani->Add(10018);
	animations->Add(503, ani);

	ani = new CAnimation(100);	// sit left
	ani->Add(10008);
	animations->Add(404, ani);
	ani = new CAnimation(100);	// sit right
	ani->Add(10018);
	animations->Add(504, ani);



	ani = new CAnimation(100);		// brick
	ani->Add(90001);
	animations->Add(601, ani);

	ani = new CAnimation(100);
	ani->Add(20001);
	ani->Add(20002);
	ani->Add(20003);
	animations->Add(600, ani);

	SIMON = new CSIMON();
	SIMON->AddAnimation(400);		// idle right 0
	SIMON->AddAnimation(401);		// idle left 1
	SIMON->AddAnimation(500);		// walk right 2
	SIMON->AddAnimation(501);		// walk left 3
	SIMON->AddAnimation(402);       //attack right 4
	SIMON->AddAnimation(502);       //attack left 5
	SIMON->AddAnimation(403);       //jump right 6
	SIMON->AddAnimation(503);       //jump left 7
	SIMON->AddAnimation(404);       //sit right 8
	SIMON->AddAnimation(504);       //sit left 9
	SIMON->SetPosition(50.0f, 0);

	objects.push_back(SIMON);


	for (int i = 0; i < 100; i++)
	{
		CBrick* brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(i * 10.0f, 160);
		objects.push_back(brick);
	}
}

void GameScene::Update(DWORD dt)
{
	// We know that SIMON is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow SIMON
	float cx, cy;
	//CGame::GetInstance()->SetCamPos(0.0f, 0.0f /*cy*/);
	SIMON->GetPosition(cx, cy);
	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;
	if (cx < 650 / 2 && cx>0)
	{
		CGame::GetInstance()->SetCamPos(cx, 0.0f);///cy
	}
	/*if (cx >=250.0f)///////
	{

		CGame::GetInstance()->SetCamPos(cx, 0.0f );///cy
	}*/
}

void GameScene::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int x = 0, y = 0;
	int flag = 1;
	int i, j;
	ifstream file_entrance("entrance.txt");
	int number;
	queue<int>entr;
	if (file_entrance.is_open())
	{
		while (!file_entrance.eof())
		{
			file_entrance >> number;
			entr.push(number);
		}
	}
	for (i = 0; i < 6 * 32; i = i + 32)
	{
		for (j = 0; j < 24 * 32; j = j + 32)
		{
			sprites->Get(entr.front())->Draw(j, i);
			entr.pop();
		}
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}
