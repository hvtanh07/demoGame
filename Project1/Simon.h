#pragma once
#include "GameObject.h"
#include "MorningStar.h"
#include "Game.h"
#include "Brick.h"

#define SIMON_WALKING_SPEED		0.1f 
#define PULL_UP_SIMON_AFTER_SITTING 18.0f
//0.1f
#define SIMON_JUMP_SPEED_Y		0.22f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.0006f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_RIGHT					100
#define SIMON_LEFT					200
//#define SIMON_STATE_SQUAT		500

#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_ATTACK 500
#define SIMON_STATE_SIT 600

#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			1

#define SIMON_ANI_WALKING_RIGHT			2
#define SIMON_ANI_WALKING_LEFT			3

#define SIMON_ANI_ATTACK_RIGHT 4
#define SIMON_ANI_ATTACK_LEFT 5

#define SIMON_ANI_JUMP_RIGHT 6
#define SIMON_ANI_JUMP_LEFT 7

#define SIMON_ANI_SIT_RIGHT 8
#define SIMON_ANI_SIT_LEFT 9

#define SIMON_ANI_DIE		99		

#define SIMON_IDLE_BBOX_WIDTH  16
#define SIMON_IDLE_BBOX_HEIGHT 30
#define SIMON_SIT_BBOX_HEIGHT 20

#define SIMON_UNTOUCHABLE_TIME 500
#define SIMON_JUMP_TIME 800
#define SIMON_ATTACK_TIME 300
#define PULL_UP_SIMON_AFTER_SITTING 10.0f;
#define PULL_UP_SIMON_AFTER_JUMP 10.0f;

class CSIMON : public CGameObject
{
	bool jump;
	bool attack;
	//int right;
	int untouchable;
	bool sit = false;

	DWORD untouchable_start;
	DWORD jump_start;
	DWORD attack_start;

	//isCollisionAxisYWithBrick = false;
public:
	CSIMON() : CGameObject()
	{
		untouchable = 0;
		jump = 0;
		attack = 0;
		//sit = false;
		/*MS = new CBrick();
		MS->SetActive(true);*/
	}
	~CSIMON() {

	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartJump() { jump = true; jump_start = GetTickCount(); }
	void StartAttack() { attack = true; attack_start = GetTickCount(); }
	bool getsit() { return sit; }
	void attacking();
	void sitdown();
	void Standup();
	void CheckCollisionWithGround(LPCOLLISIONEVENT e);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};