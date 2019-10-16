#include "Keyboard.h"
#include "debug.h"
#include "Simon.h"

void Keyboard::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		SIMON->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: // reset
		SIMON->SetState(SIMON_STATE_IDLE);
		SIMON->SetPosition(50.0f, 0.0f);
		SIMON->SetSpeed(0, 0);
		break;
	}
}

void Keyboard::OnKeyUp(int KeyCode)
{
	/*DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);*/
}

void Keyboard::KeyState(BYTE* states)
{
	// disable control key when SIMON die 
	//if (SIMON->GetState() == SIMON_STATE_DIE) return;
	////if (SIMON->GetState() == SIMON_STATE_JUMP);
	//if (game->IsKeyDown(DIK_RIGHT))
	//	SIMON->SetState(SIMON_STATE_WALKING_RIGHT);
	//else if (game->IsKeyDown(DIK_LEFT))
	//	SIMON->SetState(SIMON_STATE_WALKING_LEFT);
	//else if (game->IsKeyDown(DIK_DOWN))
	//	SIMON->SetState(SIMON_STATE_SQUAT);
	//else
	//	SIMON->SetState(SIMON_STATE_IDLE);
}
