#include "Mario.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Goomba.h"
#include "../Character/Koopa.h"
#include "../Character/Piranha.h"
#include "../Character/Venus.h"
#include "../Character/VenusFireball.h"
#include "../Item/Coin.h"
#include "../Item/ExtraLife.h"
#include "../Item/Mushroom.h"
#include "../Item/Leaf.h"
#include "../Prop/HollowedPlatform.h"
#include "../Prop/Brick.h"
#include "../Prop/Block.h"
#include "../Prop/DeadZone.h"
#include "../Prop/SpawnZone.h"
#include "../Prop/Switch.h"
#include "../Prop/Pipe.h"
#include "../Prop/Goal.h"


void CMario::Load()
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

	/* Body */
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	SMALL_BODY_WIDTH = bodyNode.attribute("width").as_float();
	SMALL_BODY_HEIGHT = bodyNode.attribute("height").as_float();
	SMALL_BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	SMALL_BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
	bodyNode = bodyNode.next_sibling("Body");
	LARGE_BODY_WIDTH = bodyNode.attribute("width").as_float();
	LARGE_BODY_HEIGHT = bodyNode.attribute("height").as_float();
	LARGE_BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	LARGE_BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	ACCELERATE = statsNode.attribute("ACCELERATE").as_float();
	FAST_ACCELERATE = statsNode.attribute("FAST_ACCELERATE").as_float();
	DRAG_FORCE = statsNode.attribute("DRAG_FORCE").as_float();
	MOVE_SPEED_LIMIT = statsNode.attribute("MOVE_SPEED_LIMIT").as_float();
	MAX_MOVE_SPEED_LIMIT = statsNode.attribute("MAX_MOVE_SPEED_LIMIT").as_float();
	POWER_LIMIT = statsNode.attribute("POWER_LIMIT").as_float();
	POWER_COUNTDOWN = statsNode.attribute("POWER_COUNTDOWN").as_float();
	DRIFT_LIMIT = statsNode.attribute("DRIFT_LIMIT").as_float();
	JUMP_FORCE = statsNode.attribute("JUMP_FORCE").as_float();
	JUMP_LIMIT = statsNode.attribute("JUMP_LIMIT").as_float();
	HOVER_FORCE = statsNode.attribute("HOVER_FORCE").as_float();
	HOVER_INTERVAL = statsNode.attribute("HOVER_INTERVAL").as_float();
	FLY_FORCE = statsNode.attribute("FLY_FORCE").as_float();
	FLY_INTERVAL = statsNode.attribute("FLY_INTERVAL").as_float();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	KICK_INTERVAL = statsNode.attribute("KICK_INTERVAL").as_float();
	INVINCIBLE_COUNTDOWN = statsNode.attribute("INVINCIBLE_COUNTDOWN").as_float();
	DEFLECT_FORCE = statsNode.attribute("DEFLECT_FORCE").as_float();
	SHELL_OFFSET = statsNode.attribute("SHELL_OFFSET").as_float();
	/* Weapon */
}

void CMario::Start()
{
	_start = true;
	_left = true;
	_hold = false;
	_ax = 0.0f;
	_ay = 0.0f;
	_powerMeter = 0;
	_powerCountdown = 0;
	_power = EPower::SMALL;
	SetAction(EAction::IDLE);
}

void CMario::Update(float elapsedMs)
{
	if (!_start) Start();

	_ay = -GRAVITY;
	if (_action == EAction::FLY
		|| _action == EAction::HOVER)
	{
		_ay = -HOVER_FORCE;
	}

	if (_game->IsPressed(2)) SetNextAction(EAction::POWERDOWN);
	else if (_game->IsPressed(3)) SetNextAction(EAction::POWERUP);
	else if (_game->IsPressed(4)) SetNextAction(EAction::POWERUP_TAIL);
	else if (_game->IsPressed(5))
	{
		SetPosition(100, 100);
		SetNextAction(EAction::IDLE);
	}

	if (_hold && _shell != nullptr)
	{
		if (_left) _shell->SetPosition(_x - SHELL_OFFSET, _y);
		else _shell->SetPosition(_x + SHELL_OFFSET, _y);
		_shell->_vy = 0;
		if (_shell->_action == CKoopa::EAction::MOVE)
		{
			Hit();
			_hold = false;
			_shell = nullptr;
		}
	}

	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;
	_vx = _vx / DRAG_FORCE;
	
	if (_invincible)
	{
		_invincibleCountdown -= elapsedMs;
		_blink = !_blink;
		if (_invincibleCountdown < 0)
		{
			_blink = false;
			_invincible = false;
		}
	}

	_powerMeter -= elapsedMs;
	if (_powerMeter < 0)
	{
		_powerMeter = 0;
	}

	if (_action != EAction::RUN && _fly)
	{
		_powerCountdown += elapsedMs;
		if (_powerCountdown > POWER_COUNTDOWN)
		{
			_powerCountdown = 0;
			_fly = false;
		}
	}

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CMario::Render()
{
	if (_renderBody)
	{
		_sprites[BBOX_SMALL]->Render(_x, _y);
		_sprites[BBOX_LARGE]->Render(_x, _y);
	}
	if (!_blink)
	{
		switch (_action)
		{
		case CMario::EAction::IDLE:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_S_HOLD_IDLE_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_S_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _sprites[SPR_MARIO_S_IDLE_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_S_IDLE_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_L_HOLD_IDLE_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_L_HOLD_IDLE_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _sprites[SPR_MARIO_L_IDLE_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_L_IDLE_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::RACCOON:
			{
				{
					if (_hold)
					{
						if (_left) _sprites[SPR_MARIO_R_HOLD_IDLE_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_R_HOLD_IDLE_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_R_IDLE_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_R_IDLE_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;
			}
			break;

		case CMario::EAction::WALK:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_hold)
				{
					if (_left) _animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_S_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_S_WALK_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_hold)
				{
					if (_left) _animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_L_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_L_WALK_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_hold)
				{
					if (_left) _animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_R_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_R_WALK_RIGHT]->Render(_x, _y);
				}
			}
			break;
			}
			break;

		case CMario::EAction::JUMP:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_S_HOLD_WALK_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_S_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _sprites[SPR_MARIO_S_JUMP_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_S_JUMP_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_L_HOLD_WALK1_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_L_HOLD_WALK1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _sprites[SPR_MARIO_L_JUMP_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_L_JUMP_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_R_HOLD_WALK1_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_R_HOLD_WALK1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _sprites[SPR_MARIO_R_JUMP_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_R_JUMP_RIGHT]->Render(_x, _y);
				}
			}
			break;
			}
			break;

		case CMario::EAction::HOVER:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_left) _animations[ANI_MARIO_R_HOVER_LEFT]->Render(_x, _y);
				else	   _animations[ANI_MARIO_R_HOVER_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::RUN:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_fly)
				{
					if (_left) _animations[ANI_MARIO_S_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_S_RUN_RIGHT]->Render(_x, _y);
				}
				else if (_hold)
				{
					if (_left) _animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_S_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_S_RUN_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_fly)
				{
					if (_left) _animations[ANI_MARIO_L_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_L_RUN_RIGHT]->Render(_x, _y);
				}
				else if (_hold)
				{
					if (_left) _animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_L_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_L_RUN_RIGHT]->Render(_x, _y);
				}
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_fly)
				{
					if (_left) _animations[ANI_MARIO_R_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_R_RUN_RIGHT]->Render(_x, _y);
				}
				else if (_hold)
				{
					if (_left) _animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_R_RUN_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_R_RUN_RIGHT]->Render(_x, _y);
				}
			}
			break;
			}
			break;

		case CMario::EAction::DRIFT:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_left) _sprites[SPR_MARIO_S_DRIFT_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_S_DRIFT_RIGHT]->Render(_x, _y);
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_left) _sprites[SPR_MARIO_L_DRIFT_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_L_DRIFT_RIGHT]->Render(_x, _y);
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_left) _sprites[SPR_MARIO_R_DRIFT_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_R_DRIFT_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::FLY:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_left) _animations[ANI_MARIO_R_FLY_LEFT]->Render(_x, _y);
				else	   _animations[ANI_MARIO_R_FLY_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::CROUNCH:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
				if (_left) _sprites[SPR_MARIO_S_IDLE_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_S_IDLE_RIGHT]->Render(_x, _y);
			}
			break;
			case CMario::EPower::LARGE:
			{
				if (_left) _sprites[SPR_MARIO_L_CROUCH_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_L_CROUCH_RIGHT]->Render(_x, _y);
			}
			break;
			case CMario::EPower::RACCOON:
			{
				if (_left) _sprites[SPR_MARIO_R_CROUCH_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_R_CROUCH_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::SPIN:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
			}
			break;
			}
			break;

		case CMario::EAction::KICK:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
			}
			break;
			}
			break;

		case CMario::EAction::POWERUP:
		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERDOWN:
		{
			if (_left) _animations[ANI_MARIO_L_GROW_LEFT]->Render(_x, _y);
			else _animations[ANI_MARIO_L_GROW_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EAction::DIE:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
			}
			break;
			}
			break;

		case CMario::EAction::PIPE_IN:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
			}
			break;
			}
			break;

		case CMario::EAction::PIPE_OUT:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			{
			}
			break;
			case CMario::EPower::LARGE:
			{
			}
			break;
			case CMario::EPower::RACCOON:
			{
			}
			break;
			}
			break;
		}
	}
}

void CMario::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CMario::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CMario::EAction::WALK:
		Walk(elapsedMs);
		break;

	case CMario::EAction::JUMP:
		Jump(elapsedMs);
		break;

	case CMario::EAction::HOVER:
		Hover(elapsedMs);
		break;

	case CMario::EAction::RUN:
		Run(elapsedMs);
		break;

	case CMario::EAction::DRIFT:
		Drift(elapsedMs);
		break;

	case CMario::EAction::FLY:
		Fly(elapsedMs);
		break;

	case CMario::EAction::CROUNCH:
		Crounch(elapsedMs);
		break;

	case CMario::EAction::SPIN:
		Spin(elapsedMs);
		break;

	case CMario::EAction::KICK:
		Kick(elapsedMs);
		break;

	case CMario::EAction::POWERUP:
		PowerUp(elapsedMs);
		break;

	case CMario::EAction::POWERUP_TAIL:
		PowerTail(elapsedMs);
		break;

	case CMario::EAction::POWERDOWN:
		PowerDown(elapsedMs);
		break;

	case CMario::EAction::DIE:
		Die(elapsedMs);
		break;

	case CMario::EAction::PIPE_IN:
		PipeIn(elapsedMs);
		break;

	case CMario::EAction::PIPE_OUT:
		PipeOut(elapsedMs);
		break;
	}
}

void CMario::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_ax = 0;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
		{
			_left = true;
			if (_game->IsDown(ACTION))
			{
				SetNextAction(EAction::RUN);
			}
			SetNextAction(EAction::WALK);
		}
		else if (_game->IsDown(RIGHT) && !_game->IsDown(LEFT))
		{
			_left = false;
			if (_game->IsDown(ACTION))
			{
				SetNextAction(EAction::RUN);
			}
			SetNextAction(EAction::WALK);
		}

		if (_game->IsDown(DOWN))
		{
			SetNextAction(EAction::CROUNCH);
		}

		if (_ground && _game->IsDown(JUMP))
		{
			SetNextAction(EAction::JUMP);
		}
		else if (!_ground && _game->IsPressed(JUMP) && _power == EPower::RACCOON)
		{
			SetNextAction(EAction::HOVER);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Walk(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Play(true);

			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Play(true);
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_WALK_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_WALK_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_WALK_RIGHT]->Update(elapsedMs);

			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Update(elapsedMs);
		}

		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}
		
		if ((!_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
			|| (_game->IsDown(LEFT) && _game->IsDown(RIGHT)))
		{
			_ax = 0;
			if (_left && _vx < DRIFT_LIMIT)
			{
				SetNextAction(EAction::IDLE);
			}
			else if (!_left && _vx > -DRIFT_LIMIT)
			{
				SetNextAction(EAction::IDLE);
			}
		}
		else if (_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
		{
			_left = true;
			if (_game->IsDown(ACTION))
			{
				SetNextAction(EAction::RUN);
			}
		}
		else if (_game->IsDown(RIGHT) && !_game->IsDown(LEFT))
		{
			_left = false;
			if (_game->IsDown(ACTION))
			{
				SetNextAction(EAction::RUN);
			}
		}

		if (_left)
		{
			_ax = -ACCELERATE;
			if (_vx < -MOVE_SPEED_LIMIT) _vx = -MOVE_SPEED_LIMIT;
			if (_vx < -DRIFT_LIMIT && _game->IsDown(RIGHT))
			{
				_left = false;
				SetNextAction(EAction::DRIFT);
			}
		}
		else
		{
			_ax = ACCELERATE;
			if (_vx > MOVE_SPEED_LIMIT) _vx = MOVE_SPEED_LIMIT;
			if (_vx > DRIFT_LIMIT && _game->IsDown(LEFT))
			{
				_left = true;
				SetNextAction(EAction::DRIFT);
			}
		}

		if (_ground && _game->IsDown(JUMP))
		{
			SetNextAction(EAction::JUMP);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_L_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_R_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_WALK_RIGHT]->Stop();

			_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Stop();
			_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Stop();
		}

		_ax = 0;
	}
	NextAction();
	break;
	}
}

void CMario::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_ground = false;
		_fall = false;
		_jumpLimit = JUMP_LIMIT;
		_vy = JUMP_FORCE;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_fall)
		{
			if (_game->IsPressed(JUMP) && _power == EPower::RACCOON)
			{
				SetNextAction(EAction::HOVER);
			}
		}

		if (_game->IsReleased(JUMP))
		{
			_fall = true;
		}
		
		if (!_fall && _jumpLimit > 0)
		{
			_jumpLimit -= elapsedMs;
			_vy = JUMP_FORCE;
		}
		else
		{
			_fall = true;
		}

		if (_game->IsDown(LEFT))
		{
			_left = true;
			{
				_ax = -ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (_game->IsDown(RIGHT))
		{
			_left = false;
			{
				_ax = ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_ax = 0;
		}
		if (_ground) SetNextAction(EAction::IDLE);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Hover(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_R_HOVER_LEFT]->Play(true);
		_animations[ANI_MARIO_R_HOVER_RIGHT]->Play(true);

		_hoverInterval = HOVER_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_R_HOVER_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_R_HOVER_RIGHT]->Update(elapsedMs);

		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_hoverInterval > 0)
		{
			_hoverInterval -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_R_HOVER_LEFT]->Stop();
		_animations[ANI_MARIO_R_HOVER_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Run(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Play(true);

			_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Play(true);
		
			_animations[ANI_MARIO_S_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_RUN_LEFT]->Play(true);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Play(true);
		
			_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Play(true);
			_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Play(true);
			_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Play(true);
			_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Play(true);
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Update(elapsedMs);

			_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);

			_animations[ANI_MARIO_S_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_RUN_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_RUN_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_RUN_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_RUN_RIGHT]->Update(elapsedMs);

			_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Update(elapsedMs);
		}

		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_powerMeter < POWER_LIMIT)
		{
			_powerMeter += elapsedMs*2;
		}
		else
		{
			_fly = true;
		}

		if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			if (_left && _vx < DRIFT_LIMIT)
			{
				SetNextAction(EAction::IDLE);
			}
			else if (!_left && _vx > -DRIFT_LIMIT)
			{
				SetNextAction(EAction::IDLE);
			}
		}
		else if (_game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT)
			&& _game->IsDown(ACTION))
		{
			_left = true;
		}
		else if (_game->IsDown(RIGHT)
			&& !_game->IsDown(LEFT)
			&& _game->IsDown(ACTION))
		{
			_left = false;
		}
		else if (!_game->IsDown(ACTION))
		{
			SetNextAction(EAction::WALK);
		}
		 

		if (_left)
		{
			_ax = -FAST_ACCELERATE;
			if (_vx < - MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			if (_vx < -DRIFT_LIMIT && _game->IsDown(RIGHT))
			{
				_left = false;
				SetNextAction(EAction::DRIFT);
			}
		}
		else
		{
			_ax = FAST_ACCELERATE;
			if (_vx > MAX_MOVE_SPEED_LIMIT) _vx = MAX_MOVE_SPEED_LIMIT;
			if (_vx > DRIFT_LIMIT && _game->IsDown(LEFT))
			{
				_left = true;
				SetNextAction(EAction::DRIFT);
			}
		}

		if (_fly && _game->IsDown(JUMP) && _power == EPower::RACCOON)
		{
			SetNextAction(EAction::FLY);
		}
		else if (_ground && _game->IsDown(JUMP))
		{
			SetNextAction(EAction::JUMP);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* animation */
		{
			_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Stop();

			_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Stop();
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Stop();
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Stop();

			_animations[ANI_MARIO_S_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_S_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_L_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_L_RUN_RIGHT]->Stop();
			_animations[ANI_MARIO_R_RUN_LEFT]->Stop();
			_animations[ANI_MARIO_R_RUN_RIGHT]->Stop();

			_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Stop();
			_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Stop();
			_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Stop();
			_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Stop();
		}

		_ax = 0;
	}
	NextAction();
	break;
	}
}

void CMario::Drift(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_left && _vx < DRIFT_LIMIT) 
		{
			SetNextAction(EAction::IDLE);
		}
		else if (!_left && _vx > -DRIFT_LIMIT)
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Fly(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_R_FLY_LEFT]->Play(true);
		_animations[ANI_MARIO_R_FLY_RIGHT]->Play(true);
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_R_FLY_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_R_FLY_RIGHT]->Update(elapsedMs);

		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (_game->IsDown(LEFT))
		{
			_left = true;
			{
				_ax = -ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (_game->IsDown(RIGHT))
		{
			_left = false;
			{
				_ax = ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_ax = 0;
		}

		if (_game->IsPressed(JUMP))
		{
			_powerCountdown -= elapsedMs/2;
			_vy = FLY_FORCE;
		}
		if (!_fly)
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_R_FLY_LEFT]->Stop();
		_animations[ANI_MARIO_R_FLY_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Crounch(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_hold && _game->IsReleased(ACTION))
		{
			SetNextAction(EAction::KICK);
		}

		if (!_game->IsDown(DOWN))
		{
			SetNextAction(EAction::IDLE);
		}

		if (_game->IsDown(DOWN) && _game->IsPressed(JUMP) && _ground)
		{
			_vy = JUMP_FORCE;
			_ground = false;
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Spin(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Kick(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		if (_shell != nullptr && _hold)
		{
			_hold = false;
			_shell->_left = _left;
			_shell->SetNextAction(CKoopa::EAction::SPIN);
			_shell = nullptr;
		}
		_kickInterval = KICK_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_kickInterval > 0)
		{
			_kickInterval -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::PowerUp(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Play(true);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Play(true);

		_power = EPower::LARGE;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Update(elapsedMs);

		if (_powerUpInterval > 0)
		{
			_powerUpInterval -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Stop();
		_animations[ANI_MARIO_L_GROW_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::PowerTail(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Play(true);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Play(true);

		_power = EPower::RACCOON;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Update(elapsedMs);

		if (_powerUpInterval > 0)
		{
			_powerUpInterval -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Stop();
		_animations[ANI_MARIO_L_GROW_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::PowerDown(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Play(true);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Play(true);

		_power = EPower::SMALL;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_L_GROW_RIGHT]->Update(elapsedMs);

		if (_powerUpInterval > 0)
		{
			_powerUpInterval -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_animations[ANI_MARIO_L_GROW_LEFT]->Stop();
		_animations[ANI_MARIO_L_GROW_RIGHT]->Stop();
	}
	NextAction();
	break;
	}
}

void CMario::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::PipeIn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::PipeOut(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
	}
	break;

	case CMario::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMario::Hit()
{
	if (_power == EPower::SMALL)
	{
		SetNextAction(CMario::EAction::DIE);
	}
	else if (_power == EPower::LARGE)
	{
		_invincible = true;
		_invincibleCountdown = INVINCIBLE_COUNTDOWN;
		_vy = DEFLECT_FORCE;
		if(_left) _vx = DEFLECT_FORCE;
		else _vx = -DEFLECT_FORCE;
		SetNextAction(CMario::EAction::POWERDOWN);
	}
	else if (_power == EPower::RACCOON)
	{
		_invincible = true;
		_invincibleCountdown = INVINCIBLE_COUNTDOWN;
		_vy = DEFLECT_FORCE;
		if (_left) _vx = DEFLECT_FORCE;
		else _vx = -DEFLECT_FORCE;
		SetNextAction(CMario::EAction::POWERDOWN);
	}

}

int CMario::IsCollidable()
{
	switch (_action)
	{
	case CMario::EAction::IDLE:
	case CMario::EAction::WALK:
	case CMario::EAction::JUMP:
	case CMario::EAction::HOVER:
	case CMario::EAction::RUN:
	case CMario::EAction::DRIFT:
	case CMario::EAction::FLY:
	case CMario::EAction::CROUNCH:
	case CMario::EAction::SPIN:
	case CMario::EAction::KICK:
	case CMario::EAction::POWERUP:
	case CMario::EAction::POWERUP_TAIL:
	case CMario::EAction::POWERDOWN:
		return 1;
		break;

	case CMario::EAction::DIE:
	case CMario::EAction::PIPE_IN:
	case CMario::EAction::PIPE_OUT:
		return 0;
		break;
	}
	return 0;
}

int CMario::IsBlocking()
{
    return 0;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_power)
	{
	case CMario::EPower::SMALL:
		switch (_action)
		{
		case CMario::EAction::IDLE:
		case CMario::EAction::WALK:
		case CMario::EAction::JUMP:
		case CMario::EAction::HOVER:
		case CMario::EAction::RUN:
		case CMario::EAction::DRIFT:
		case CMario::EAction::FLY:
		case CMario::EAction::CROUNCH:
		case CMario::EAction::SPIN:
		case CMario::EAction::KICK:
		case CMario::EAction::POWERUP:
		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERDOWN:
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
			break;

		case CMario::EAction::DIE:
		case CMario::EAction::PIPE_IN:
		case CMario::EAction::PIPE_OUT:
			left = 0;
			right = 0;
			top = 0;
			bottom = 0;
			break;
		}
		break;

	case CMario::EPower::LARGE:
	case CMario::EPower::RACCOON:
		switch (_action)
		{
		case CMario::EAction::IDLE:
		case CMario::EAction::WALK:
		case CMario::EAction::JUMP:
		case CMario::EAction::HOVER:
		case CMario::EAction::RUN:
		case CMario::EAction::DRIFT:
		case CMario::EAction::FLY:
		case CMario::EAction::SPIN:
		case CMario::EAction::KICK:
		case CMario::EAction::POWERUP:
		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERDOWN:
			left = _x + LARGE_BODY_OFFSETX - (LARGE_BODY_WIDTH / 2);
			right = _x + LARGE_BODY_OFFSETX + (LARGE_BODY_WIDTH / 2);
			top = _y + LARGE_BODY_OFFSETY + (LARGE_BODY_HEIGHT / 2);
			bottom = _y + LARGE_BODY_OFFSETY - (LARGE_BODY_HEIGHT / 2);
			break;

		case CMario::EAction::CROUNCH:
			left = _x + SMALL_BODY_OFFSETX - (SMALL_BODY_WIDTH / 2);
			right = _x + SMALL_BODY_OFFSETX + (SMALL_BODY_WIDTH / 2);
			top = _y + SMALL_BODY_OFFSETY + (SMALL_BODY_HEIGHT / 2);
			bottom = _y + SMALL_BODY_OFFSETY - (SMALL_BODY_HEIGHT / 2);
			break;

		case CMario::EAction::DIE:
		case CMario::EAction::PIPE_IN:
		case CMario::EAction::PIPE_OUT:
			left = 0;
			right = 0;
			top = 0;
			bottom = 0;
			break;
		}
		break;
	}
}

void CMario::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
	_ground = false;
}

void CMario::OnCollisionWith(pCollision e)
{
	if (e->_ny != 0 && e->_target->IsBlocking())
	{
		_vy = 0;
		if (e->_ny > 0)	_ground = true;
	}
	if (e->_nx != 0 && e->_target->IsBlocking())
	{
		_ax = 0;
		_vx = 0;
	}

	if (dynamic_cast<pHollowedPlatform>(e->_target))
	{
		if (e->_ny > 0 && !e->_target->IsBlocking())
		{
			float top = 0;
			float temp = 0;
			e->_target->GetBoundingBox(temp, top, temp, temp);
			_y = top + BLOCK_PUSH_FACTOR;
			_vy = 0;
			_ground = true;
		}
	}
	/* Items */
	else if (dynamic_cast<pMushroom>(e->_target))
	{
		pMushroom mushroom = dynamic_cast<pMushroom>(e->_target);
		if (_power == EPower::SMALL)
		{
			SetNextAction(EAction::POWERUP);
		}
		mushroom->SetNextAction(CMushroom::EAction::CONSUMED);
	}
	else if (dynamic_cast<pCoin>(e->_target))
	{
		pCoin coin = dynamic_cast<pCoin>(e->_target);
		coin->SetNextAction(CCoin::EAction::CONSUMED);
	}
	else if (dynamic_cast<pExtraLife>(e->_target))
	{
		pExtraLife extraLife = dynamic_cast<pExtraLife>(e->_target);
		extraLife->SetNextAction(CExtraLife::EAction::CONSUMED);
	}
	else if (dynamic_cast<pLeaf>(e->_target))
	{
		pLeaf leaf = dynamic_cast<pLeaf>(e->_target);
		if (_power != EPower::RACCOON)
		{
			SetNextAction(EAction::POWERUP_TAIL);
		}
		leaf->SetNextAction(CLeaf::EAction::CONSUMED);
	}

	/* Character */
	else if (dynamic_cast<pGoomba>(e->_target))
	{
		pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
		if (e->_ny > 0)
		{
			if (goomba->_wing) goomba->_wing = false;
			else goomba->SetNextAction(CGoomba::EAction::DIE);
			_vy = DEFLECT_FORCE;
		}
		else if (e->_nx != 0)
		{
			if (!_invincible) Hit();
		}
	}
	else if (dynamic_cast<pKoopa>(e->_target))
	{
		pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
		if (e->_ny > 0)
		{
			if (koopa->_wing) koopa->_wing = false;
			else koopa->SetNextAction(CKoopa::EAction::RETRACT);
			_vy = DEFLECT_FORCE;

			if (koopa->_action == CKoopa::EAction::RETRACT)
			{
				koopa->SetNextAction(CKoopa::EAction::SPIN);
			}
		}
		else if (e->_nx != 0)
		{
			if (koopa->_action == CKoopa::EAction::RETRACT)
			{
				if (!_hold && _game->IsDown(ACTION))
				{
					_shell = koopa;
					_hold = true;
				}
				else
				{
					if (e->_nx > 0) koopa->_left = true;
					else koopa->_left = false;
					koopa->SetNextAction(CKoopa::EAction::SPIN);
					SetNextAction(CMario::EAction::KICK);
				}
			}
			else
			{
				if (!_invincible)
				{
					Hit();
					_vy = DEFLECT_FORCE;
					_shell = nullptr;
					_hold = false;
				}
			}
		}
	}

	else if (dynamic_cast<pPiranha>(e->_target))
	{
		pPiranha piranha = dynamic_cast<pPiranha>(e->_target);
		if (!_invincible) Hit();
		_vy = DEFLECT_FORCE;
	}

	else if (dynamic_cast<pVenus>(e->_target))
	{
		pVenus venus = dynamic_cast<pVenus>(e->_target);
		if (!_invincible) Hit();
		_vy = DEFLECT_FORCE;
	}

	else if (dynamic_cast<pVenusFireball>(e->_target))
	{
		pVenusFireball venusFireball = dynamic_cast<pVenusFireball>(e->_target);
		if (!_invincible) Hit();
		_vy = DEFLECT_FORCE;
	}

	/* Props */
	else if (dynamic_cast<pBrick>(e->_target))
	{
		pBrick brick = dynamic_cast<pBrick>(e->_target);
		if (e->_ny < 0)
		{
			_vy = - DEFLECT_FORCE/2;
			if (_power != EPower::SMALL) brick->SetNextAction(CBrick::EAction::BOUNCE);
			else brick->SetNextAction(CBrick::EAction::BROKE);
		}
	}
	else if (dynamic_cast<pBlock>(e->_target))
	{
		pBlock block = dynamic_cast<pBlock>(e->_target);
		if (e->_ny < 0)
		{
			_vy = - DEFLECT_FORCE / 2;
			if (block->_action != CBlock::EAction::EMTPY)
			{
				block->_topBounce = true;
				block->_leftBounce = false;
				block->SetNextAction(CBlock::EAction::SPAWN);
			}
		}
	}
	else if (dynamic_cast<pSwitch>(e->_target))
	{
	pSwitch relay = dynamic_cast<pSwitch>(e->_target);

	}
	else if (dynamic_cast<pDeadZone>(e->_target))
	{
	pDeadZone deadZone = dynamic_cast<pDeadZone>(e->_target);

	}
}

