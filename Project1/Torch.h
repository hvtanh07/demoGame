#pragma once
#include "GameObject.h"
#include "MorningStar.h"

#define TORCH_WALKING_SPEED 0.05f;

#define TORCH_BBOX_WIDTH 16
#define TORCH_BBOX_HEIGHT 30

#define TORCH_STATE_ALIVE 100
#define TORCH_STATE_DESTROYED 200

#define TORCH_ANI_WALKING 0
#define TORCH_ANI_DIE 1

class Torch : public CGameObject
{
	boolean active = true;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CheckCollisionWithMorningStar(LPCOLLISIONEVENT e);
};

