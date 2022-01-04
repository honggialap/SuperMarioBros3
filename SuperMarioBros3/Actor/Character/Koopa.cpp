#include "Koopa.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Goomba.h"
#include "../Character/Koopa.h"
#include "../Character/Piranha.h"
#include "../Character/Venus.h"
#include "../Prop/Brick.h"
#include "../Prop/Block.h"
#include "../Prop/HollowedPlatform.h"

void CKoopa::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Sensor */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("GameObject");
	_sensor = dynamic_cast<pKoopaSensor>(
		_game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			_name.append(sensorNode.attribute("name").as_string()),
			sensorNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		)
	);

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	_type = EType(statsNode.attribute("type").as_int());
	_wing = statsNode.attribute("wing").as_bool();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	WALK_SPEED = statsNode.attribute("WALK_SPEED").as_float();
	SPIN_SPEED = statsNode.attribute("SPIN_SPEED").as_float();
	JUMP_FORCE = statsNode.attribute("JUMP_FORCE").as_float();
	RESTORE_LIMIT = statsNode.attribute("RESTORE_LIMIT").as_float();
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
}

void CKoopa::Start()
{
	_start = true;
	_left = true;
	_ax = 0.0f;
	_ay = 0.0f;
	_sensor->Active();
	SetAction(EAction::MOVE);
}

void CKoopa::Update(float elapsedMs)
{
	if (!_start) Start();

	_ay = -GRAVITY;
	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;

	if (_sensor != nullptr)
	{
		_sensor->SetPosition(_x, _y);
		if (_sensor->_activate
			&& _type == EType::RED_KOOPA
			&& _action == EAction::MOVE
			&& _ground)
		{
			_left = !_left;
		}
		_sensor->_left = _left;
	}

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);

	HandleAction(elapsedMs);
}

void CKoopa::Render()
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			if (_left) _animations[ANI_KOOPA_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_KOOPA_WALK_RIGHT]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			if (_left) _animations[ANI_RED_KOOPA_WALK_LEFT]->Render(_x, _y);
			else _animations[ANI_RED_KOOPA_WALK_RIGHT]->Render(_x, _y);
			break;
		}
		if (_wing)
		{
			if (_left) _animations[ANI_KOOPA_WING_LEFT]->Render(_x, _y);
			else _animations[ANI_KOOPA_WING_RIGHT]->Render(_x, _y);
		}
	}
		break;
	case CKoopa::EAction::RETRACT:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_BLINK_DOWN]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK_DOWN]->Render(_x, _y);
			break;
		}
	}
		break;
	case CKoopa::EAction::SPIN:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_SPIN_DOWN]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN_DOWN]->Render(_x, _y);
			break;
		}
	}
		break;
	case CKoopa::EAction::DIE:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_sprites[SPR_KOOPA_RETRACT1_DOWN]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_sprites[SPR_RED_KOOPA_RETRACT1_DOWN]->Render(_x, _y);
			break;
		}
	}
		break;
	}
}

void CKoopa::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
		Move(elapsedMs);
		break;
	case CKoopa::EAction::RETRACT:
		Retract(elapsedMs);
		break;
	case CKoopa::EAction::SPIN:
		Spin(elapsedMs);
		break;
	case CKoopa::EAction::DIE:
		Die(elapsedMs);
		break;
	}
}

void CKoopa::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::START:
	{
		if (_wing)
		{
			_animations[ANI_KOOPA_WING_LEFT]->Play(true);
			_animations[ANI_KOOPA_WING_RIGHT]->Play(true);
		}
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_WALK_LEFT]->Play(true);
			_animations[ANI_KOOPA_WALK_RIGHT]->Play(true);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_WALK_LEFT]->Play(true);
			_animations[ANI_RED_KOOPA_WALK_RIGHT]->Play(true);
			break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_wing)
		{
			_animations[ANI_KOOPA_WING_LEFT]->Update(elapsedMs);
			_animations[ANI_KOOPA_WING_RIGHT]->Update(elapsedMs);
		}
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_KOOPA_WALK_RIGHT]->Update(elapsedMs);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_WALK_LEFT]->Update(elapsedMs);
			_animations[ANI_RED_KOOPA_WALK_RIGHT]->Update(elapsedMs);
			break;
		}

		if (_left)
		{
			_vx = -WALK_SPEED;
		}
		else
		{
			_vx = WALK_SPEED;
		}

		if (_ground && _wing)
		{
			_vy = JUMP_FORCE;
			_ground = false;
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		if (_wing)
		{
			_animations[ANI_KOOPA_WING_LEFT]->Stop();
			_animations[ANI_KOOPA_WING_RIGHT]->Stop();
		}
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_WALK_LEFT]->Stop();
			_animations[ANI_KOOPA_WALK_RIGHT]->Stop();
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_WALK_LEFT]->Stop();
			_animations[ANI_RED_KOOPA_WALK_RIGHT]->Stop();
			break;
		}
		_vx = 0;
		_vy = 0;
	}
	NextAction();
	break;
	}
}

void CKoopa::Retract(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::START:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_BLINK_DOWN]->Play(true);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK_DOWN]->Play(true);
			break;
		}
		_restoreLimit = RESTORE_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_BLINK_DOWN]->Update(elapsedMs);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK_DOWN]->Update(elapsedMs);
			break;
		}

		if (_restoreLimit > 0)
		{
			_restoreLimit -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::MOVE);
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_BLINK_DOWN]->Stop();
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK_DOWN]->Stop();
			break;
		}
	}
	NextAction();
	break;
	}
}


void CKoopa::Spin(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::START:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_SPIN_DOWN]->Play(true);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN_DOWN]->Play(true);
			break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_SPIN_DOWN]->Update(elapsedMs);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN_DOWN]->Update(elapsedMs);
			break;
		}	

		if (_left)
		{
			_vx = -SPIN_SPEED;
		}
		else
		{
			_vx = SPIN_SPEED;
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_SPIN_DOWN]->Stop();
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN_DOWN]->Stop();
			break;
		}
		_vx = 0;
	}
	NextAction();
	break;
	}
}

void CKoopa::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::START:
	{
		_vy = JUMP_FORCE;
		_decayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_decayInterval > 0)
		{
			_decayInterval -= elapsedMs;
		}
		else
		{
			_start = false;
			_sensor->Destroy();
			_sensor = nullptr;
			Destroy();
		}
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CKoopa::IsCollidable()
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	case CKoopa::EAction::RETRACT:
	case CKoopa::EAction::SPIN:
		return 1	;
		break;
	case CKoopa::EAction::DIE:
		return 0;
		break;
	}
	return 0;
}

int CKoopa::IsBlocking()
{
	return 0;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CKoopa::EAction::MOVE:
	case CKoopa::EAction::RETRACT:
	case CKoopa::EAction::SPIN:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;

	case CKoopa::EAction::DIE:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CKoopa::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CKoopa::OnCollisionWith(pCollision e)
{
	if (e->_ny != 0 && e->_target->IsBlocking())
	{
		_vy = 0;
		if (e->_ny > 0) _ground = true;
	}
	if (e->_nx != 0 && e->_target->IsBlocking())
	{
		_vx = 0;
		_left = !_left;
	}

	if (dynamic_cast<pHollowedPlatform>(e->_target))
	{
		if (e->_ny > 0)
		{
			float top = 0;
			float temp = 0;
			e->_target->GetBoundingBox(temp, top, temp, temp);
			_y = top + BLOCK_PUSH_FACTOR;
			_vy = 0;
			_ground = true;
		}
	}

	/* Character */
	else if (dynamic_cast<pGoomba>(e->_target))
	{
		pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
		if (_action == EAction::SPIN)
		{
			if (goomba->_wing) goomba->_wing = false;
			goomba->SetNextAction(CGoomba::EAction::THROWN);
		}
	}
	else if (dynamic_cast<pKoopa>(e->_target))
	{
		pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
		if (_action == EAction::SPIN)
		{
			if(koopa->_wing) koopa->_wing = false;
			koopa->SetNextAction(CKoopa::EAction::DIE);
		}
	}

	else if (dynamic_cast<pPiranha>(e->_target))
	{
		pPiranha piranha = dynamic_cast<pPiranha>(e->_target);
		if (_action == EAction::SPIN) piranha->SetNextAction(CPiranha::EAction::DIE);
	}

	else if (dynamic_cast<pVenus>(e->_target))
	{
		pVenus venus = dynamic_cast<pVenus>(e->_target);
		if (_action == EAction::SPIN) venus->SetNextAction(CVenus::EAction::DIE);
	}

	/* Props */
	else if (dynamic_cast<pBrick>(e->_target))
	{
		pBrick brick = dynamic_cast<pBrick>(e->_target);
		if (e->_nx != 0)
		{
			if (_action == EAction::SPIN) 
				brick->SetNextAction(CBrick::EAction::BROKE);
		}
	}
	else if (dynamic_cast<pBlock>(e->_target))
	{
		pBlock block = dynamic_cast<pBlock>(e->_target);
		if (e->_nx != 0)
		{
			if (_action == EAction::SPIN)
			{
				if (block->_action != CBlock::EAction::EMTPY)
				{
					block->_topBounce = false;
					if (e->_nx > 0) block->_leftBounce = true;
					else block->_leftBounce = false;
					block->SetNextAction(CBlock::EAction::SPAWN);
				}
			}
		}
	}
}
