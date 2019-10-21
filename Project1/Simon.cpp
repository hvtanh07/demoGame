#include <algorithm>
#include "debug.h"

#include "SIMON.h"
#include "Game.h"
#include "Brick.h"


void CSIMON::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 

	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	
	if (GetTickCount() - jump_start > SIMON_JUMP_TIME)
	{
		jump_start = 0;
		jump = false;
		//Standup();
	}
	if (GetTickCount() - attack_start > SIMON_ATTACK_TIME)
	{
		attack_start = 0;
		attack = 0;
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			CheckCollisionWithGround(e);
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSIMON::Render()
{
	int ani= SIMON_STATE_IDLE;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_IDLE)
	{
		if (vx == 0)
		{
			if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;
		}
	}
	else if (state == SIMON_RIGHT)
	{
		if (!sit)ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_SIT_RIGHT;
	}
	else if (state == SIMON_LEFT)
	{
		if (!sit)ani = SIMON_ANI_WALKING_LEFT;
		else ani = SIMON_ANI_SIT_LEFT;
	}
	else if (state == SIMON_STATE_SIT)
	{
		if (nx > 0)
		{
			ani = SIMON_ANI_SIT_RIGHT;
		}
		else ani = SIMON_ANI_SIT_LEFT;
	}
	
	/*if (attack != 0)
	{
		if (nx > 0)
			ani = SIMON_ANI_ATTACK_RIGHT;
		else ani = SIMON_ANI_ATTACK_LEFT;
	}*/
	if (jump != 0)
	{
		if (nx > 0)
			ani = SIMON_ANI_JUMP_RIGHT;
		else ani = SIMON_ANI_JUMP_LEFT;
	}
	int alpha = 255;
	animations[ani]->Render(x, y, alpha);
	RenderBoundingBox();
}

void CSIMON::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_RIGHT:
		setnx(1);
		if (!sit) vx = SIMON_WALKING_SPEED;
		break;
	case SIMON_LEFT:
		setnx(-1);
		if (!sit)vx = -SIMON_WALKING_SPEED;
		break;
	case SIMON_STATE_JUMP:
		if (jump == false) {
			vy = -SIMON_JUMP_SPEED_Y;
			StartJump();
		}
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_SIT:
		vx = 0;
		if (!sit) sitdown();
		
		break;
	}
}
void CSIMON::sitdown() {
	y += PULL_UP_SIMON_AFTER_SITTING;
	sit = true;
}
void CSIMON::Standup()
{
	y = y - PULL_UP_SIMON_AFTER_SITTING;
	sit = false;
	jump = false;
}

void CSIMON::CheckCollisionWithGround(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Brick
	{
		CBrick* brick = dynamic_cast<CBrick*>(e->obj);
		if (jump)
		{
			jump = false;
			y = y - PULL_UP_SIMON_AFTER_JUMP;
		}
	}
}

void CSIMON::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_IDLE_BBOX_WIDTH;
	bottom = y + SIMON_IDLE_BBOX_HEIGHT;
	
	if (sit||jump)
	{
		bottom = y + SIMON_SIT_BBOX_HEIGHT;
	}
}

