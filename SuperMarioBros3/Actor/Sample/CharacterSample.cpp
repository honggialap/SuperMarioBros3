#include "CharacterSample.h"
#include "GameSample.h"

#include "Engine/Framework/Debug.h"
#include "Engine/Library/pugixml.hpp"


void CCharacterSample::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Command Binding */
	pugi::xml_node bindingNode = prefab.child("Prefab").child("Binding");
	UP = bindingNode.attribute("UP").as_int();
	DOWN = bindingNode.attribute("DOWN").as_int();
	LEFT = bindingNode.attribute("LEFT").as_int();
	RIGHT = bindingNode.attribute("RIGHT").as_int();
	JUMP = bindingNode.attribute("JUMP").as_int();
	ACTION = bindingNode.attribute("ACTION").as_int();

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	MOVE_SPEED = statsNode.attribute("MOVE_SPEED").as_float();
	JUMP_FORCE = statsNode.attribute("JUMP_FORCE").as_float();
	DRAG_FORCE = statsNode.attribute("DRAG_FORCE").as_float();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	/* Weapon */

}

void CCharacterSample::Start()
{
	_start = true;
	SetAction(EAction::IDLE);
	_game->MoveCameraTo(0, 0);
	SetPosition(32, 960);

}

void CCharacterSample::Update(float elapsedMs)
{
	if (!_start) Start();

	/* Physics */
	_ay -= GRAVITY;

	_vx += (_ax * elapsedMs) - (_vx / DRAG_FORCE);
	_vy += (_ay * elapsedMs) - (_vy / DRAG_FORCE);




	/* Collision */




	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;

	/* State */

}

void CCharacterSample::Render()
{
	switch (_action)
	{

	case CCharacterSample::EAction::IDLE:
	{
		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
			if (_isLeft) _sprites[SPR_CHARA_S_IDLE_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_S_IDLE_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::LARGE:
			if (_isLeft) _sprites[SPR_CHARA_L_IDLE_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_L_IDLE_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::FIRE:
			if (_isLeft) _sprites[SPR_CHARA_F_IDLE_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_F_IDLE_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::RACCOON:
			if (_isLeft) _sprites[SPR_CHARA_R_IDLE_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_R_IDLE_RIGHT]->Render(_x, _y);
			break;
		}
	}
	break;

	case CCharacterSample::EAction::WALK:
	{
		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
			if (_isLeft) _animations[ANI_CHARA_S_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_CHARA_S_WALK_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::LARGE:
			if (_isLeft) _animations[ANI_CHARA_L_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_CHARA_L_WALK_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::FIRE:
			if (_isLeft) _animations[ANI_CHARA_F_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_CHARA_F_WALK_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::RACCOON:
			if (_isLeft) _animations[ANI_CHARA_R_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_CHARA_R_WALK_RIGHT]->Render(_x, _y);
			break;
		}
	}
	break;

	case CCharacterSample::EAction::JUMP:
	{
		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
			if (_isLeft) _sprites[SPR_CHARA_S_JUMP_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_S_JUMP_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::LARGE:
			if (_isLeft) _sprites[SPR_CHARA_L_JUMP_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_L_JUMP_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::FIRE:
			if (_isLeft) _sprites[SPR_CHARA_F_JUMP_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_F_JUMP_RIGHT]->Render(_x, _y);
			break;
		case CCharacterSample::EPower::RACCOON:
			if (_isLeft) _sprites[SPR_CHARA_R_JUMP_LEFT]->Render(_x, _y);
			else _sprites[SPR_CHARA_R_JUMP_RIGHT]->Render(_x, _y);
			break;
		}
	}
	break;
	}
}

void CCharacterSample::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CCharacterSample::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CCharacterSample::EAction::WALK:
		Walk(elapsedMs);
		break;

	case CCharacterSample::EAction::JUMP:
		Jump(elapsedMs);
		break;
	}
}

void CCharacterSample::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CCharacterSample::EActionStage::START:
	{
		_ax = 0;
		_actionStage = EActionStage::PROGRESS;
	}
	break;

	case CCharacterSample::EActionStage::PROGRESS:
	{
		if (!_game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_isLeft = false;
			SetNextAction(EAction::WALK);
		}
		else if (_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
		{
			_isLeft = true;
			SetNextAction(EAction::WALK);
		}
	}
	break;

	case CCharacterSample::EActionStage::EXIT:
	{
		NextAction();
	}
	break;
	}
}

void CCharacterSample::Walk(float elapsedMs)
{
	switch (_actionStage)
	{
	case CCharacterSample::EActionStage::START:
	{
		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
		{
			_animations[ANI_CHARA_S_WALK_LEFT]->Play(true);
			_animations[ANI_CHARA_S_WALK_RIGHT]->Play(true);
		}
		break;
		case CCharacterSample::EPower::LARGE:
		{
			_animations[ANI_CHARA_L_WALK_LEFT]->Play(true);
			_animations[ANI_CHARA_L_WALK_RIGHT]->Play(true);
		}
		break;
		case CCharacterSample::EPower::FIRE:
		{
			_animations[ANI_CHARA_F_WALK_LEFT]->Play(true);
			_animations[ANI_CHARA_F_WALK_RIGHT]->Play(true);
		}
		break;
		case CCharacterSample::EPower::RACCOON:
		{
			_animations[ANI_CHARA_R_WALK_LEFT]->Play(true);
			_animations[ANI_CHARA_R_WALK_RIGHT]->Play(true);

		}
		break;
		}
		_actionStage = EActionStage::PROGRESS;
	}
	break;

	case CCharacterSample::EActionStage::PROGRESS:
	{
		if (!_game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_isLeft = false;
			_ax = MOVE_SPEED;
		}
		else if (_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
		{
			_isLeft = true;
			_ax = -MOVE_SPEED;
		}
		else if ((!_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
			|| (_game->IsDown(LEFT) && _game->IsDown(RIGHT)))
		{
			SetNextAction(EAction::IDLE);
		}

		if (_game->IsDown(JUMP))
		{
			SetNextAction(EAction::JUMP);
		}

		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
			if (_isLeft) _animations[ANI_CHARA_S_WALK_LEFT]->Update(elapsedMs);
			else _animations[ANI_CHARA_S_WALK_RIGHT]->Update(elapsedMs);
			break;
		case CCharacterSample::EPower::LARGE:
			if (_isLeft) _animations[ANI_CHARA_L_WALK_LEFT]->Update(elapsedMs);
			else _animations[ANI_CHARA_L_WALK_RIGHT]->Update(elapsedMs);
			break;
		case CCharacterSample::EPower::FIRE:
			if (_isLeft) _animations[ANI_CHARA_F_WALK_LEFT]->Update(elapsedMs);
			else _animations[ANI_CHARA_F_WALK_RIGHT]->Update(elapsedMs);
			break;
		case CCharacterSample::EPower::RACCOON:
			if (_isLeft) _animations[ANI_CHARA_R_WALK_LEFT]->Update(elapsedMs);
			else _animations[ANI_CHARA_R_WALK_RIGHT]->Update(elapsedMs);
			break;
		}
	}
	break;

	case CCharacterSample::EActionStage::EXIT:
	{
		switch (_power)
		{
		case CCharacterSample::EPower::SMALL:
		{
			_animations[ANI_CHARA_S_WALK_LEFT]->Stop();
			_animations[ANI_CHARA_S_WALK_RIGHT]->Stop();
		}
		break;
		case CCharacterSample::EPower::LARGE:
		{
			_animations[ANI_CHARA_L_WALK_LEFT]->Stop();
			_animations[ANI_CHARA_L_WALK_RIGHT]->Stop();
		}
		break;
		case CCharacterSample::EPower::FIRE:
		{
			_animations[ANI_CHARA_F_WALK_LEFT]->Stop();
			_animations[ANI_CHARA_F_WALK_RIGHT]->Stop();
		}
		break;
		case CCharacterSample::EPower::RACCOON:
		{
			_animations[ANI_CHARA_R_WALK_LEFT]->Stop();
			_animations[ANI_CHARA_R_WALK_RIGHT]->Stop();

		}
		break;
		}
		NextAction();
	}
	break;
	}
}

void CCharacterSample::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CCharacterSample::EActionStage::START:
	{
	}
	break;

	case CCharacterSample::EActionStage::PROGRESS:
	{
	}
	break;

	case CCharacterSample::EActionStage::EXIT:
	{
		NextAction();
	}
	break;
	}
}

void CCharacterSample::OnCollisionWith(pCollision e)
{
}

void CCharacterSample::OnCollisionWithItem(pCollision e)
{
}

