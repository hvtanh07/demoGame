#include "Torch.h"
void Torch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TORCH_BBOX_WIDTH;
	b = y + TORCH_BBOX_HEIGHT;
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (active)
		CalcPotentialCollisions(coObjects, coEvents);

	float min_tx, min_ty, nx = 0, ny;

	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];
		CheckCollisionWithMorningStar(e);
	}
}

void Torch::Render()
{
	if (active != true)
		return;
	animations[0]->Render(x, y);
	RenderBoundingBox();
}

void Torch::SetState(int state)
{
	CGameObject::SetState(state);

}

void Torch::CheckCollisionWithMorningStar(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<MorningStar*>(e->obj)) // if e->obj is Brick
	{
		//MorningStar* ms = dynamic_cast<MorningStar*>(e->obj);
		active = false;
	}
}
