#include "Mario.h"
#include "../../SuperMarioBros3.h"
#include "../Controller/GameMaster.h"
#include "../Controller/World.h"
#include "../Character/MarioFireball.h"
#include "../Character/Goomba.h"
#include "../Character/Koopa.h"
#include "../Character/Piranha.h"
#include "../Character/Venus.h"
#include "../Character/VenusFireball.h"
#include "../Item/Coin.h"
#include "../Item/ExtraLife.h"
#include "../Item/Mushroom.h"
#include "../Item/Leaf.h"
#include "../Item/Flower.h"
#include "../Prop/Platform.h"
#include "../Prop/Brick.h"
#include "../Prop/Block.h"
#include "../Prop/DeadZone.h"
#include "../Prop/SpawnZone.h"
#include "../Prop/Switch.h"
#include "../Prop/Pipe.h"
#include "../Prop/TransportPipe.h"
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
	HOTKEY1 = bindingNode.attribute("HOTKEY1").as_int();
	HOTKEY2 = bindingNode.attribute("HOTKEY2").as_int();
	HOTKEY3 = bindingNode.attribute("HOTKEY3").as_int();
	HOTKEY4 = bindingNode.attribute("HOTKEY4").as_int();
	HOTKEY5 = bindingNode.attribute("HOTKEY5").as_int();
	
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
	_controllerName = statsNode.attribute("controllerName").as_string();
	_gameMasterName = statsNode.attribute("gameMasterName").as_string();

	/* Stats - Movement */
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

	/* Stats - Action */
	POWERUP_INTERVAL = statsNode.attribute("POWERUP_INTERVAL").as_float();
	INVINCIBLE_COUNTDOWN = statsNode.attribute("INVINCIBLE_COUNTDOWN").as_float();
	KICK_INTERVAL = statsNode.attribute("KICK_INTERVAL").as_float();
	DEFLECT_FORCE = statsNode.attribute("DEFLECT_FORCE").as_float();
	SHELL_OFFSET = statsNode.attribute("SHELL_OFFSET").as_float();
	TAIL_ENTRY = statsNode.attribute("TAIL_ENTRY").as_float();
	TAIL_PROGRESS = statsNode.attribute("TAIL_PROGRESS").as_float();
	TAIL_RECOVER = statsNode.attribute("TAIL_RECOVER").as_float();
	FIRE_ENTRY = statsNode.attribute("FIRE_ENTRY").as_float();
	FIRE_PROGRESS = statsNode.attribute("FIRE_PROGRESS").as_float();
	FIRE_RECOVER = statsNode.attribute("FIRE_RECOVER").as_float();
	FIRE_OFFSETX = statsNode.attribute("FIRE_OFFSETX").as_float();
	FIRE_OFFSETY = statsNode.attribute("FIRE_OFFSETY").as_float();
	PIPE_MOVEMENT = statsNode.attribute("PIPE_MOVEMENT").as_float();
	DIE_ENTRY = statsNode.attribute("DIE_ENTRY").as_float();
	DIE_PROGRESS = statsNode.attribute("DIE_PROGRESS").as_float();

	/* Weapon */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("Tail");
	std::string tailName = _name + sensorNode.attribute("name").as_string();
	_tail = dynamic_cast<pMarioTail>(
		_game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			tailName,
			sensorNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		)
	);
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

	if ((_action != EAction::PIPE_IN)
		&& (_action != EAction::PIPE_OUT))
	{
		_vx += _ax * elapsedMs;
		_vy += _ay * elapsedMs;
		_vx = _vx / DRAG_FORCE;
	}

	HotCheat();
	UpdateShell();
	UpdateTail();
	UpdatePowerBar(elapsedMs);
	UpdateInvincible(elapsedMs);

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

			case CMario::EPower::FIRE:
			{
				{
					if (_hold)
					{
						if (_left) _sprites[SPR_MARIO_F_HOLD_IDLE_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_F_HOLD_IDLE_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_F_IDLE_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_F_IDLE_RIGHT]->Render(_x, _y);
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

			case CMario::EPower::FIRE:
			{
				if (_hold)
				{
					if (_left) _animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_left) _animations[ANI_MARIO_F_WALK_LEFT]->Render(_x, _y);
					else	   _animations[ANI_MARIO_F_WALK_RIGHT]->Render(_x, _y);
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
					if (_fly)
					{
						if (_left) _sprites[SPR_MARIO_S_FLY_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_S_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_S_JUMP_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_S_JUMP_RIGHT]->Render(_x, _y);
					}
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
					if (_fly)
					{
						if (_left) _sprites[SPR_MARIO_L_FLY_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_L_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_L_JUMP_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_L_JUMP_RIGHT]->Render(_x, _y);
					}
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
					if (_fly)
					{
						if (_left) _sprites[SPR_MARIO_R_FLY1_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_R_FLY1_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_R_JUMP_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_R_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;

			case CMario::EPower::FIRE:
			{
				if (_hold)
				{
					if (_left) _sprites[SPR_MARIO_F_HOLD_WALK1_LEFT]->Render(_x, _y);
					else	   _sprites[SPR_MARIO_F_HOLD_WALK1_RIGHT]->Render(_x, _y);
				}
				else
				{
					if (_fly)
					{
						if (_left) _sprites[SPR_MARIO_F_FLY_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_F_FLY_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _sprites[SPR_MARIO_F_JUMP_LEFT]->Render(_x, _y);
						else	   _sprites[SPR_MARIO_F_JUMP_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;

			}
			break;

		case CMario::EAction::HOVER:
			switch (_power)
			{
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
				if (_hold)
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_S_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_S_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_S_RUN_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;

			case CMario::EPower::LARGE:
			{
				if (_hold)
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_L_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_L_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_L_RUN_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;

			case CMario::EPower::RACCOON:
			{
				if (_hold)
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_R_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_R_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_R_RUN_RIGHT]->Render(_x, _y);
					}
				}
			}
			break;

			case CMario::EPower::FIRE:
			{
				if (_hold)
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_F_HOLD_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_F_HOLD_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_F_HOLD_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_F_HOLD_RUN_RIGHT]->Render(_x, _y);
					}
				}
				else
				{
					if (_fly)
					{
						if (_left) _animations[ANI_MARIO_F_RUN_FULL_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_F_RUN_FULL_RIGHT]->Render(_x, _y);
					}
					else
					{
						if (_left) _animations[ANI_MARIO_F_RUN_LEFT]->Render(_x, _y);
						else	   _animations[ANI_MARIO_F_RUN_RIGHT]->Render(_x, _y);
					}
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

			case CMario::EPower::FIRE:
			{
				if (_left) _sprites[SPR_MARIO_F_DRIFT_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_F_DRIFT_RIGHT]->Render(_x, _y);
			}
			break;

			}
			break;

		case CMario::EAction::FLY:
			switch (_power)
			{
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

			case CMario::EPower::FIRE:
			{
				if (_left) _sprites[SPR_MARIO_F_CROUCH_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_F_CROUCH_RIGHT]->Render(_x, _y);
			}
			break;

			}
			break;

		case CMario::EAction::SPIN:
			switch (_power)
			{
			case CMario::EPower::RACCOON:
			{
				if (_left) _animations[ANI_MARIO_R_ATTACK_LEFT]->Render(_x, _y);
				else	   _animations[ANI_MARIO_R_ATTACK_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::SHOOT:
			switch (_power)
			{
			case CMario::EPower::FIRE:
			{
				if (_left) _animations[ANI_MARIO_F_ATTACK_LEFT]->Render(_x, _y);
				else	   _animations[ANI_MARIO_F_ATTACK_RIGHT]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::KICK:
			switch (_power)
			{

			case CMario::EPower::SMALL:
			{
				if (_left) _sprites[SPR_MARIO_S_KICK_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_S_KICK_RIGHT]->Render(_x, _y);
			}
			break;

			case CMario::EPower::LARGE:
			{
				if (_left) _sprites[SPR_MARIO_L_KICK_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_L_KICK_RIGHT]->Render(_x, _y);
			}
			break;

			case CMario::EPower::RACCOON:
			{
				if (_left) _sprites[SPR_MARIO_R_KICK_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_R_KICK_RIGHT]->Render(_x, _y);
			}
			break;

			case CMario::EPower::FIRE:
			{
				if (_left) _sprites[SPR_MARIO_F_KICK_LEFT]->Render(_x, _y);
				else	   _sprites[SPR_MARIO_F_KICK_RIGHT]->Render(_x, _y);
			}
			break;

			}
			break;

		case CMario::EAction::POWERUP:
		{
			if (_left)	_animations[ANI_MARIO_L_GROW_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_L_GROW_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERUP_FIRE:
		{
			_animations[ANI_MARIO_R_TRANSFORM]->Render(_x, _y);
		}
		break;

		case CMario::EAction::POWERDOWN:
		{
			if (_left)	_animations[ANI_MARIO_L_DEGROW_LEFT]->Render(_x, _y);
			else		_animations[ANI_MARIO_L_DEGROW_RIGHT]->Render(_x, _y);
		}
		break;

		case CMario::EAction::DIE:
			switch (_power)
			{
			case CMario::EPower::SMALL:
			case CMario::EPower::LARGE:
			case CMario::EPower::RACCOON:
			case CMario::EPower::FIRE:
			{
				_sprites[SPR_MARIO_S_DIE]->Render(_x, _y);
			}
			break;
			}
			break;

		case CMario::EAction::PIPE_OUT:
		case CMario::EAction::PIPE_IN:
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_sprites[SPR_MARIO_S_FRONT]->Render(_x, _y);
				break;
			case CMario::EPower::LARGE:
				_sprites[SPR_MARIO_L_FRONT]->Render(_x, _y);
				break;
			case CMario::EPower::RACCOON:
				_sprites[SPR_MARIO_R_FRONT]->Render(_x, _y);
				break;
			case CMario::EPower::FIRE:
				_sprites[SPR_MARIO_F_FRONT]->Render(_x, _y);
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

	case CMario::EAction::SHOOT:
		Shoot(elapsedMs);
		break;

	case CMario::EAction::POWERUP:
		PowerUp(elapsedMs);
		break;

	case CMario::EAction::POWERUP_TAIL:
		PowerTail(elapsedMs);
		break;

	case CMario::EAction::POWERUP_FIRE:
		PowerFire(elapsedMs);
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
		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		else if (_power == EPower::FIRE
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SHOOT);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (_game->IsDown(DOWN))
			SetNextAction(EAction::CROUNCH);

		if (_ground
			&& _game->IsDown(JUMP))
			SetNextAction(EAction::JUMP);

		else if (!_ground
			&& _game->IsPressed(JUMP)
			&& _power == EPower::RACCOON)
			SetNextAction(EAction::HOVER);

		if (_game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT))
		{
			_left = true;
			SetNextAction(EAction::WALK);

			if (_game->IsDown(ACTION))
				SetNextAction(EAction::RUN);
		}

		else if (_game->IsDown(RIGHT)
			&& !_game->IsDown(LEFT))
		{
			_left = false;
			SetNextAction(EAction::WALK);

			if (_game->IsDown(ACTION))
				SetNextAction(EAction::RUN);
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
		/* Animation Start */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_S_WALK_RIGHT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Play(true);
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_L_WALK_RIGHT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Play(true);
				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_R_WALK_RIGHT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Play(true);
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_F_WALK_RIGHT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Play(true);
				break;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_WALK_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_WALK_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_WALK_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_WALK_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Update(elapsedMs);
				break;
			}
		}

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (_game->IsDown(DOWN))
			SetNextAction(EAction::CROUNCH);

		if ((!_game->IsDown(LEFT) && !_game->IsDown(RIGHT))
			|| (_game->IsDown(LEFT) && _game->IsDown(RIGHT)))
		{
			_ax = 0;
			if ((_left && _vx < DRIFT_LIMIT)
				|| (!_left && _vx > -DRIFT_LIMIT))
				SetNextAction(EAction::IDLE);
		}
		else if (_game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT))
		{
			_left = true;

			if (_game->IsDown(ACTION))
				SetNextAction(EAction::RUN);

			if (_power == EPower::RACCOON
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SPIN);

			else if (_power == EPower::FIRE
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SHOOT);
		}
		else if (_game->IsDown(RIGHT)
			&& !_game->IsDown(LEFT))
		{
			_left = false;

			if (_game->IsDown(ACTION))
				SetNextAction(EAction::RUN);

			if (_power == EPower::RACCOON
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SPIN);

			else if (_power == EPower::FIRE
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SHOOT);
		}

		if (_left)
		{
			_ax = -ACCELERATE;
			if (_vx < -MOVE_SPEED_LIMIT) _vx = -MOVE_SPEED_LIMIT;
			if (_vx < -DRIFT_LIMIT
				&& _game->IsDown(RIGHT))
			{
				_left = false;
				SetNextAction(EAction::DRIFT);
			}
		}
		else
		{
			_ax = ACCELERATE;
			if (_vx > MOVE_SPEED_LIMIT) _vx = MOVE_SPEED_LIMIT;
			if (_vx > DRIFT_LIMIT
				&& _game->IsDown(LEFT))
			{
				_left = true;
				SetNextAction(EAction::DRIFT);
			}
		}

		if (_ground
			&& _game->IsDown(JUMP))
			SetNextAction(EAction::JUMP);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_S_WALK_RIGHT]->Stop();
				_animations[ANI_MARIO_S_HOLD_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_S_HOLD_WALK_RIGHT]->Stop();
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_L_WALK_RIGHT]->Stop();
				_animations[ANI_MARIO_L_HOLD_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_L_HOLD_WALK_RIGHT]->Stop();
				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_R_WALK_RIGHT]->Stop();
				_animations[ANI_MARIO_R_HOLD_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_R_HOLD_WALK_RIGHT]->Stop();
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_F_WALK_RIGHT]->Stop();
				_animations[ANI_MARIO_F_HOLD_WALK_LEFT]->Stop();
				_animations[ANI_MARIO_F_HOLD_WALK_RIGHT]->Stop();
				break;
			}
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
		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		else if (_power == EPower::FIRE
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SHOOT);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (_fall
			&& _game->IsPressed(JUMP)
			&& _power == EPower::RACCOON)
			SetNextAction(EAction::HOVER);

		if (_game->IsReleased(JUMP))
			_fall = true;

		if (!_fall
			&& _jumpLimit > 0)
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
				if (_vx > MAX_MOVE_SPEED_LIMIT) _vx = MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_ax = 0;
		}

		if (_ground)
			SetNextAction(EAction::IDLE);
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
		/* Animation Start */
		{
			_animations[ANI_MARIO_R_HOVER_LEFT]->Play(false);
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Play(false);
		}

		_hoverInterval = HOVER_INTERVAL;
		_vy = 0;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_R_HOVER_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Update(elapsedMs);
		}

		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		else if (_power == EPower::FIRE
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SHOOT);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (_hoverInterval > 0) _hoverInterval -= elapsedMs;
		else SetNextAction(EAction::IDLE);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			_animations[ANI_MARIO_R_HOVER_LEFT]->Stop();
			_animations[ANI_MARIO_R_HOVER_RIGHT]->Stop();
		}
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
		/* Animation Start */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_S_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Play(true);
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_L_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Play(true);
				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_R_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Play(true);
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_F_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_F_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_F_RUN_FULL_RIGHT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_RUN_LEFT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_RUN_RIGHT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_LEFT]->Play(true);
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_RIGHT]->Play(true);
				break;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_RUN_FULL_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_RUN_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_RUN_RIGHT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_LEFT]->Update(elapsedMs);
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_RIGHT]->Update(elapsedMs);
				break;
			}
		}

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (_game->IsDown(DOWN))
			SetNextAction(EAction::CROUNCH);

		if (_powerMeter < POWER_LIMIT) _powerMeter += elapsedMs * 2;
		else _fly = true;

		if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			if ((_left && _vx < DRIFT_LIMIT)
				|| (!_left && _vx > -DRIFT_LIMIT))
				SetNextAction(EAction::IDLE);
		}
		else if (_game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT)
			&& _game->IsDown(ACTION))
		{
			_left = true;

			if (_power == EPower::RACCOON
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SPIN);

			else if (_power == EPower::FIRE
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SHOOT);
		}
		else if (_game->IsDown(RIGHT)
			&& !_game->IsDown(LEFT)
			&& _game->IsDown(ACTION))
		{
			_left = false;

			if (_power == EPower::RACCOON
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SPIN);

			else if (_power == EPower::FIRE
				&& _game->IsPressed(ACTION))
				SetNextAction(EAction::SHOOT);
		}
		else if (!_game->IsDown(ACTION))
			SetNextAction(EAction::WALK);

		if (_left)
		{
			_ax = -FAST_ACCELERATE;
			if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
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
		/* Animation Stop */
		{
			switch (_power)
			{
			case CMario::EPower::SMALL:
				_animations[ANI_MARIO_S_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_S_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_S_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_S_RUN_FULL_RIGHT]->Stop();
				_animations[ANI_MARIO_S_HOLD_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_S_HOLD_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_S_HOLD_RUN_FULL_RIGHT]->Stop();
				break;

			case CMario::EPower::LARGE:
				_animations[ANI_MARIO_L_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_L_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_L_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_L_RUN_FULL_RIGHT]->Stop();
				_animations[ANI_MARIO_L_HOLD_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_L_HOLD_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_L_HOLD_RUN_FULL_RIGHT]->Stop();

				break;

			case CMario::EPower::RACCOON:
				_animations[ANI_MARIO_R_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_R_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_R_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_R_RUN_FULL_RIGHT]->Stop();
				_animations[ANI_MARIO_R_HOLD_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_R_HOLD_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_R_HOLD_RUN_FULL_RIGHT]->Stop();
				break;

			case CMario::EPower::FIRE:
				_animations[ANI_MARIO_F_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_F_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_F_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_F_RUN_FULL_RIGHT]->Stop();
				_animations[ANI_MARIO_F_HOLD_RUN_LEFT]->Stop();
				_animations[ANI_MARIO_F_HOLD_RUN_RIGHT]->Stop();
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_LEFT]->Stop();
				_animations[ANI_MARIO_F_HOLD_RUN_FULL_RIGHT]->Stop();
				break;
			}
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
		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		else if (_power == EPower::FIRE
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SHOOT);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if ((_left && _vx < DRIFT_LIMIT)
			|| (!_left && _vx > -DRIFT_LIMIT))
			SetNextAction(EAction::IDLE);
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
		/* Animation Start */
		{
			_animations[ANI_MARIO_R_FLY_LEFT]->Play(true);
			_animations[ANI_MARIO_R_FLY_RIGHT]->Play(true);
		}

		_vy = FLY_FORCE;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			_animations[ANI_MARIO_R_FLY_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_FLY_RIGHT]->Update(elapsedMs);
		}

		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

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
				if (_vx > MAX_MOVE_SPEED_LIMIT) _vx = MAX_MOVE_SPEED_LIMIT;
			}
		}
		else if (!_game->IsDown(LEFT) && !_game->IsDown(RIGHT)
			|| _game->IsDown(LEFT) && _game->IsDown(RIGHT))
		{
			_ax = 0;
		}

		if (_game->IsPressed(JUMP))
		{
			_powerCountdown -= elapsedMs / 2;
			_vy = FLY_FORCE;
		}

		if (!_fly)
			SetNextAction(EAction::IDLE);
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			_animations[ANI_MARIO_R_FLY_LEFT]->Stop();
			_animations[ANI_MARIO_R_FLY_RIGHT]->Stop();
		}
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
		if (_power == EPower::RACCOON
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SPIN);

		else if (_power == EPower::FIRE
			&& _game->IsPressed(ACTION))
			SetNextAction(EAction::SHOOT);

		if (_hold
			&& _game->IsReleased(ACTION))
			SetNextAction(EAction::KICK);

		if (!_game->IsDown(DOWN))
			SetNextAction(EAction::IDLE);

		if (_ground
			&& _game->IsDown(DOWN)
			&& _game->IsPressed(JUMP))
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
		/* Animation Start */
		{
			_animations[ANI_MARIO_R_ATTACK_LEFT]->Play(false);
			_animations[ANI_MARIO_R_ATTACK_RIGHT]->Play(false);
		}

		_tailEntry = TAIL_ENTRY;
		_tailProgress = TAIL_PROGRESS;
		_tailRecover = TAIL_RECOVER;
		_tail->_activate = false;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

		/* Animation Start */
		{
			_animations[ANI_MARIO_R_ATTACK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_R_ATTACK_RIGHT]->Update(elapsedMs);
		}

		if (_tailEntry > 0) 
			_tailEntry -= elapsedMs;
		else
		{
			if (_tailProgress > 0)
			{
				_tailProgress -= elapsedMs;
				_tail->_activate = true;
			}
			else
			{
				_tail->_activate = false;
				if (_tailRecover > 0) 
					_tailRecover -= elapsedMs;
				else 
					SetNextAction(EAction::IDLE);
			}
		}

		_ax = 0;
		if (_left
			&& _game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT))
		{
			if (_game->IsDown(ACTION))
			{
				_ax = -FAST_ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
			else
			{
				_ax = -ACCELERATE;
				if (_vx < -MOVE_SPEED_LIMIT) _vx = -MOVE_SPEED_LIMIT;
			}
		}

		else if(!_left
			&& !_game->IsDown(LEFT)
			&& _game->IsDown(RIGHT))
		{
			if (_game->IsDown(ACTION))
			{
				_ax = FAST_ACCELERATE;
				if (_vx > MAX_MOVE_SPEED_LIMIT) _vx = MAX_MOVE_SPEED_LIMIT;
			}
			else
			{
				_ax = ACCELERATE;
				if (_vx > MOVE_SPEED_LIMIT) _vx = MOVE_SPEED_LIMIT;
			}
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Start */
		{
			_animations[ANI_MARIO_R_ATTACK_LEFT]->Stop();
			_animations[ANI_MARIO_R_ATTACK_RIGHT]->Stop();
		}
	}
	NextAction();
	break;
	}
}

void CMario::Shoot(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		/* Animation Start */
		{
			_animations[ANI_MARIO_F_ATTACK_LEFT]->Play(false);
			_animations[ANI_MARIO_F_ATTACK_RIGHT]->Play(false);
		}

		_fireEntry = FIRE_ENTRY;
		_fireProgress = FIRE_PROGRESS;
		_fireRecover = FIRE_RECOVER;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{

		/* Animation Start */
		{
			_animations[ANI_MARIO_F_ATTACK_LEFT]->Update(elapsedMs);
			_animations[ANI_MARIO_F_ATTACK_RIGHT]->Update(elapsedMs);
		}

		if (_fireEntry > 0)
			_fireEntry -= elapsedMs;
		else
		{
			if (_fireProgress > 0)
			{
				_fireProgress -= elapsedMs;
				if (!_shot)
				{
					_shot = true;
					ShootFireball();
				}
			}
			else
			{
				if (_fireProgress > 0)
					_fireProgress -= elapsedMs;
				else
				{
					_shot = false;
					SetNextAction(EAction::IDLE);
				}
			}
		}

		_ax = 0;
		if (_left
			&& _game->IsDown(LEFT)
			&& !_game->IsDown(RIGHT))
		{
			if (_game->IsDown(ACTION))
			{
				_ax = -FAST_ACCELERATE;
				if (_vx < -MAX_MOVE_SPEED_LIMIT) _vx = -MAX_MOVE_SPEED_LIMIT;
			}
			else
			{
				_ax = -ACCELERATE;
				if (_vx < -MOVE_SPEED_LIMIT) _vx = -MOVE_SPEED_LIMIT;
			}
		}
		
		else if (!_left
			&& !_game->IsDown(LEFT)
			&& _game->IsDown(RIGHT))
		{
			if (_game->IsDown(ACTION))
			{
				_ax = FAST_ACCELERATE;
				if (_vx > MAX_MOVE_SPEED_LIMIT) _vx = MAX_MOVE_SPEED_LIMIT;
			}
			else
			{
				_ax = ACCELERATE;
				if (_vx > MOVE_SPEED_LIMIT) _vx = MOVE_SPEED_LIMIT;
			}
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		/* Animation Start */
		{
			_animations[ANI_MARIO_F_ATTACK_LEFT]->Stop();
			_animations[ANI_MARIO_F_ATTACK_RIGHT]->Stop();
		}
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

		if (!_invincible) _invincible = true;
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
		_invincible = false;
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
		_animations[ANI_MARIO_R_TRANSFORM]->Play(true);

		if(!_invincible) _invincible = true;
		_power = EPower::RACCOON;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_R_TRANSFORM]->Update(elapsedMs);

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
		_animations[ANI_MARIO_R_TRANSFORM]->Stop();
		_invincible = false;
	}
	NextAction();
	break;
	}
}

void CMario::PowerFire(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		_animations[ANI_MARIO_R_TRANSFORM]->Play(true);

		if(!_invincible) _invincible = true;
		_power = EPower::FIRE;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_R_TRANSFORM]->Update(elapsedMs);

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
		_animations[ANI_MARIO_R_TRANSFORM]->Stop();
		_invincible = false;
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
		_animations[ANI_MARIO_L_DEGROW_LEFT]->Play(true);
		_animations[ANI_MARIO_L_DEGROW_RIGHT]->Play(true);

		_power = EPower::SMALL;
		_powerUpInterval = POWERUP_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		_animations[ANI_MARIO_L_DEGROW_LEFT]->Update(elapsedMs);
		_animations[ANI_MARIO_L_DEGROW_RIGHT]->Update(elapsedMs);

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
		_animations[ANI_MARIO_L_DEGROW_LEFT]->Stop();
		_animations[ANI_MARIO_L_DEGROW_RIGHT]->Stop();
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
		_vy = 0;
		_vx = 0;
		_dieEntry = DIE_ENTRY;
		_dieProgress = DIE_PROGRESS;
		_diePush = false;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_dieEntry > 0)
		{
			_vy = 0;
			_dieEntry -= elapsedMs;
		}
		else
		{
			if (!_diePush)
			{
				_vy = DEFLECT_FORCE;
				_diePush = true;
			}
			if (_dieProgress > 0) _dieProgress -= elapsedMs;
			else
			{
				auto gameMaster = dynamic_cast<pGameMaster>(_game->Get(_gameMasterName));
				gameMaster->Restart();
			}
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

void CMario::PipeIn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMario::EActionStage::START:
	{
		if (_pipe->_up) _pipeLimit = _pipe->_y - PIPE_MOVEMENT;
		else _pipeLimit = _pipe->_y + _pipe->BODY_HEIGHT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_pipe->_up)
		{
			if (_y > _pipeLimit) _y -= MOVE_SPEED_LIMIT/4 * elapsedMs;
			else SetNextAction(EAction::PIPE_OUT);
		}
		else
		{
			if (_y < _pipeLimit) _y += MOVE_SPEED_LIMIT/4 * elapsedMs;
			else SetNextAction(EAction::PIPE_OUT);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		SetPosition(_pipe->DESTINATION_X, _pipe->DESTINATION_Y);

		pWorld world = dynamic_cast<pWorld>(_game->Get(_controllerName));
		world->SetCamera(_pipe->CAMERA_SWITCH);
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
		if (_pipe->_up) _pipeLimit = _pipe->DESTINATION_Y - PIPE_MOVEMENT;
		else _pipeLimit = _pipe->DESTINATION_Y + PIPE_MOVEMENT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMario::EActionStage::PROGRESS:
	{
		if (_pipe->_up)
		{
			if (_y > _pipeLimit) _y -= MOVE_SPEED_LIMIT / 4 * elapsedMs;
			else SetNextAction(EAction::IDLE);
		}
		else
		{
			if (_y < _pipeLimit) _y += MOVE_SPEED_LIMIT / 4 * elapsedMs;
			else SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CMario::EActionStage::EXIT:
	{
		_pipe = nullptr;
	}
	NextAction();
	break;
	}
}

void CMario::HotCheat()
{
	if (_game->IsPressed(HOTKEY1)) 
		SetNextAction(EAction::POWERDOWN);

	else if (_game->IsPressed(HOTKEY2))
		SetNextAction(EAction::POWERUP);

	else if (_game->IsPressed(HOTKEY3))
		SetNextAction(EAction::POWERUP_TAIL);

	else if (_game->IsPressed(HOTKEY4))
		SetNextAction(EAction::POWERUP_FIRE);

	else if (_game->IsPressed(HOTKEY5))
	{
		auto gameMaster = dynamic_cast<pGameMaster>(_game->Get(_gameMasterName));
		gameMaster->Restart();
	}
}

void CMario::UpdateTail()
{
	_tail->_left = _left;
	_tail->SetPosition(_x, _y);
}

void CMario::UpdateShell()
{
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
}

void CMario::UpdatePowerBar(float elapsedMs)
{
	_powerMeter -= elapsedMs;
	if (_powerMeter < 0) _powerMeter = 0;
	
	if (_action != EAction::RUN && _fly)
	{
		_powerCountdown += elapsedMs;
		if (_powerCountdown > POWER_COUNTDOWN)
		{
			_powerCountdown = 0;
			_fly = false;
		}
	}
}

void CMario::UpdateInvincible(float elapsedMs)
{
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
}

void CMario::Hit()
{
	if (!_invincible) 
	{
		switch (_power)
		{
		case CMario::EPower::SMALL:
			SetNextAction(CMario::EAction::DIE);
			break;
		case CMario::EPower::LARGE:
		case CMario::EPower::RACCOON:
		case CMario::EPower::FIRE:
		{
			_invincible = true;
			_invincibleCountdown = INVINCIBLE_COUNTDOWN;
			_vy = DEFLECT_FORCE;
			if (_left) _vx = DEFLECT_FORCE;
			else _vx = -DEFLECT_FORCE;
			SetNextAction(CMario::EAction::POWERDOWN);
		}
			break;
		}
	}
}

void CMario::ShootFireball()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Fireball */
	for (auto fireballNode = prefab.child("Prefab").child("Fireball");
		fireballNode;
		fireballNode = fireballNode.next_sibling("Fireball"))
	{
		std::string fireballName = _name + fireballNode.attribute("name").as_string();
		if (_game->Get(fireballName) == nullptr)
		{
			float fireOffsetX = 0;
			if (_left) fireOffsetX = -FIRE_OFFSETX;
			else fireOffsetX = FIRE_OFFSETX;

			pMarioFireball fireball = dynamic_cast<pMarioFireball>(
				_game->Create(
				_scene,
				fireballNode.attribute("actor").as_uint(),
				fireballName,
				fireballNode.attribute("source").as_string(),
				_x + fireOffsetX, _y + FIRE_OFFSETY, _gridX, _gridY, _layer, _active
				)
				);

			if (_left) fireball->_vx = -fireball->INITIAL_FORCE;
			else fireball->_vx = fireball->INITIAL_FORCE;

			break;
		}
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
	case CMario::EAction::SHOOT:
	case CMario::EAction::KICK:
	case CMario::EAction::POWERUP:
	case CMario::EAction::POWERUP_TAIL:
	case CMario::EAction::POWERUP_FIRE:
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
		case CMario::EAction::SHOOT:
		case CMario::EAction::KICK:
		case CMario::EAction::POWERUP:
		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERUP_FIRE:
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
	case CMario::EPower::FIRE:
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
		case CMario::EAction::SHOOT:
		case CMario::EAction::KICK:
		case CMario::EAction::POWERUP:
		case CMario::EAction::POWERUP_TAIL:
		case CMario::EAction::POWERUP_FIRE:
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
	if ((_action != EAction::PIPE_IN)
		&& (_action != EAction::PIPE_OUT))
	{
		_x += _vx * elapsedMs;
		_y += _vy * elapsedMs;
		_ground = false;
	}
}

void CMario::OnCollisionWith(pCollision e)
{
	/* Blocking */
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

	/* Character */
	if (dynamic_cast<pGoomba>(e->_target))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<pKoopa>(e->_target))
		OnCollisionWithKoopa(e);

	else if (dynamic_cast<pPiranha>(e->_target))
		OnCollisionWithPiranha(e);

	else if (dynamic_cast<pVenus>(e->_target))
		OnCollisionWithVenus(e);

	else if (dynamic_cast<pVenusFireball>(e->_target))
		OnCollisionWithVenusFireball(e);

	/* Item */
	else if (dynamic_cast<pCoin>(e->_target))
		OnCollisionWithCoin(e);

	else if (dynamic_cast<pExtraLife>(e->_target))
		OnCollisionWithExtraLife(e);

	else if (dynamic_cast<pMushroom>(e->_target))
		OnCollisionWithMushroom(e);

	else if (dynamic_cast<pLeaf>(e->_target))
		OnCollisionWithLeaf(e);

	else if (dynamic_cast<pFlower>(e->_target))
		OnCollisionWithFlower(e);

	/* Prop*/
	if (dynamic_cast<pPlatform>(e->_target))
		OnCollisionWithPlatform(e);

	else if (dynamic_cast<pPipe>(e->_target))
		OnCollisionWithPipe(e);

	else if (dynamic_cast<pTransportPipe>(e->_target))
		OnCollisionWithTransportPipe(e);

	else if (dynamic_cast<pBrick>(e->_target))
		OnCollisionWithBrick(e);

	else if (dynamic_cast<pBlock>(e->_target))
		OnCollisionWithBlock(e);

	else if (dynamic_cast<pSwitch>(e->_target))
		OnCollisionWithSwitch(e);

	else if (dynamic_cast<pDeadZone>(e->_target))
		OnCollisionWithDeadZone(e);
	
}

void CMario::OnCollisionWithGoomba(pCollision e)
{
	pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
	if (goomba->_action != CGoomba::EAction::DIE
		&& goomba->_action != CGoomba::EAction::THROWN)
	{
		if (e->_ny > 0)
		{
			goomba->HitTop();
			_vy = DEFLECT_FORCE;
		}
		else if (e->_nx != 0)
		{
			if(_action != EAction::SPIN) Hit();
		}
	}
}

void CMario::OnCollisionWithKoopa(pCollision e)
{
	pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
	if (e->_ny > 0)
	{
		koopa->HitTop();
		_vy = DEFLECT_FORCE;
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
			if (_action != EAction::SPIN) Hit();
			_shell = nullptr;
			_hold = false;
		}
	}
}

void CMario::OnCollisionWithPiranha(pCollision e)
{
	pPiranha piranha = dynamic_cast<pPiranha>(e->_target);
	if (_action != EAction::SPIN) Hit();
}

void CMario::OnCollisionWithVenus(pCollision e)
{
	pVenus venus = dynamic_cast<pVenus>(e->_target);
	if (_action != EAction::SPIN) Hit();
}

void CMario::OnCollisionWithVenusFireball(pCollision e)
{
	pVenusFireball venusFireball = dynamic_cast<pVenusFireball>(e->_target);
	Hit();
	venusFireball->Destroy();
}

void CMario::OnCollisionWithPlatform(pCollision e)
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

void CMario::OnCollisionWithPipe(pCollision e)
{
	float pipeTop = 0;
	float pipeLeft = 0;
	float pipeRight = 0;
	float pipeBottom = 0;
	e->_target->GetBoundingBox(pipeLeft, pipeTop, pipeRight, pipeBottom);
	
	float top = 0;
	float left = 0;
	float right = 0;
	float bottom = 0;
	GetBoundingBox(left, top, right, bottom);

	if (e->_ny == 0 && e->_nx != 0)
	{
		if (e->_nx > 0)
		{
			_vx = 0;
			_x = pipeRight + ((right - left) / 2) + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_vx = 0;
			_x = pipeLeft - ((right - left) / 2) - BLOCK_PUSH_FACTOR;
		}
	}
	else if (e->_ny != 0 && e->_nx == 0)
	{

		if (e->_ny > 0)
		{
			_vy = 0;
			_ground = true;
			_y = pipeTop + BLOCK_PUSH_FACTOR;
		}
		else
		{
			_vy = 0;
			_y = pipeBottom - (top - bottom) - BLOCK_PUSH_FACTOR;
		}
	}
}

void CMario::OnCollisionWithTransportPipe(pCollision e)
{
	pTransportPipe transportPipe = dynamic_cast<pTransportPipe>(e->_target);
	if ((transportPipe->_up && _game->IsDown(DOWN))
		|| (!transportPipe->_up && _game->IsDown(UP)))
	{
		_pipe = transportPipe;
		SetNextAction(EAction::PIPE_IN);
	}
}

void CMario::OnCollisionWithBrick(pCollision e)
{
	pBrick brick = dynamic_cast<pBrick>(e->_target);
	if (e->_ny < 0)
	{
		_vy = -DEFLECT_FORCE / 2;
		_fall = true;
		if (brick->_action == CBrick::EAction::IDLE)
		{
			if (_power == EPower::SMALL)
				brick->SetNextAction(CBrick::EAction::BOUNCE);
			else
				brick->SetNextAction(CBrick::EAction::BROKE);
		}
	}
}

void CMario::OnCollisionWithBlock(pCollision e)
{
	pBlock block = dynamic_cast<pBlock>(e->_target);
	if (e->_ny < 0)
	{
		_vy = -DEFLECT_FORCE / 2;
		_fall = true;
		if (block->_action == CBlock::EAction::IDLE)
		{
			block->_topBounce = true;
			block->_leftBounce = false;
			block->SetNextAction(CBlock::EAction::SPAWN);
		}
	}
}

void CMario::OnCollisionWithDeadZone(pCollision e)
{
	SetNextAction(EAction::DIE);
}

void CMario::OnCollisionWithSwitch(pCollision e)
{
	pSwitch relay = dynamic_cast<pSwitch>(e->_target);
	if (e->_ny > 0)
	{
		if (!relay->_activate)
		{
			relay->_activate = true;
			relay->Transform();
		}
	}
}

void CMario::OnCollisionWithCoin(pCollision e)
{
	pCoin coin = dynamic_cast<pCoin>(e->_target);
	coin->SetNextAction(CCoin::EAction::CONSUMED);
}

void CMario::OnCollisionWithExtraLife(pCollision e)
{
	pExtraLife extraLife = dynamic_cast<pExtraLife>(e->_target);
	extraLife->SetNextAction(CExtraLife::EAction::CONSUMED);
}

void CMario::OnCollisionWithMushroom(pCollision e)
{
	pMushroom mushroom = dynamic_cast<pMushroom>(e->_target);
	if (_power == EPower::SMALL)
		SetNextAction(EAction::POWERUP);

	mushroom->SetNextAction(CMushroom::EAction::CONSUMED);
}

void CMario::OnCollisionWithLeaf(pCollision e)
{
	pLeaf leaf = dynamic_cast<pLeaf>(e->_target);
	if (_power != EPower::RACCOON)
		SetNextAction(EAction::POWERUP_TAIL);
	leaf->SetNextAction(CLeaf::EAction::CONSUMED);
}

void CMario::OnCollisionWithFlower(pCollision e)
{
	pFlower flower = dynamic_cast<pFlower>(e->_target);
	if (_power != EPower::FIRE)
		SetNextAction(EAction::POWERUP_FIRE);
	flower->SetNextAction(CFlower::EAction::CONSUMED);
}
