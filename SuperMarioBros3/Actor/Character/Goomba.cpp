#include "Goomba.h"
#include "../../SuperMarioBros3.h"
#include "../Prop/HollowedPlatform.h"

void CGoomba::Load()
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

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	_type = EType(statsNode.attribute("type").as_int());
	_wing = statsNode.attribute("wing").as_bool();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	WALK_SPEED = statsNode.attribute("WALK_SPEED").as_float();
	JUMP_FORCE = statsNode.attribute("JUMP_FORCE").as_float();
	JUMP_INTERVAL = statsNode.attribute("JUMP_INTERVAL").as_float();
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
}

void CGoomba::Start()
{
	_start = true;
	_left = true;
	_ax = 0.0f;
	_ay = 0.0f;
	SetAction(EAction::WALK);
}

void CGoomba::Update(float elapsedMs)
{
	if (!_start) Start();

	_ay = -GRAVITY;
	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CGoomba::Render()
{
	switch (_action)
	{
	case CGoomba::EAction::WALK:
	case CGoomba::EAction::JUMP:
		if(_renderBody) _sprites[BBOX]->Render(_x, _y);
	case CGoomba::EAction::THROWN:
		if (_wing)
		{
			_animations[ANI_GOOMBA_WING_FLAP]->Render(_x, _y);
		}
		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
		{
			_animations[ANI_GOOMBA_WALK]->Render(_x, _y);
		}
		break;

		case CGoomba::EType::RED_GOOMBA:
		{
			_animations[ANI_RED_GOOMBA_WALK]->Render(_x, _y);
		}
		break;
		}
		break;

	case CGoomba::EAction::DIE:
		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
		{
			_sprites[SPR_GOOMBA_DIE]->Render(_x, _y);
		}
		break;

		case CGoomba::EType::RED_GOOMBA:
		{
			_sprites[SPR_RED_GOOMBA_DIE]->Render(_x, _y);
		}
		break;
		}
		break;
	}
}

void CGoomba::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CGoomba::EAction::WALK:
		Walk(elapsedMs);
		break;

	case CGoomba::EAction::JUMP:
		Jump(elapsedMs);
		break;

	case CGoomba::EAction::DIE:
		Die(elapsedMs);
		break;

	case CGoomba::EAction::THROWN:
		Thrown(elapsedMs);
		break;
	}
}

void CGoomba::Walk(float elapsedMs)
{
	switch (_actionStage)
	{
	case CGoomba::EActionStage::START:
	{
		if (_wing)
		{
			float targetX = _game->Get(_targetId)->_x;
			if (targetX < _x)
			{
				_left = true;
			}
			else
			{
				_left = false;
			}
			_currentJumpInterval = JUMP_INTERVAL;
		}
		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Play(true);
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Play(true);
			break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CGoomba::EActionStage::PROGRESS:
	{
		if (_wing)
		{
			if (_currentJumpInterval > 0)
			{
				_currentJumpInterval -= elapsedMs;
			}
			else
			{
				SetNextAction(EAction::JUMP);
			}
		}

		if (!_left)
		{
			_vx = -WALK_SPEED;
		}
		else
		{
			_vx = WALK_SPEED;
		}

		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Update(elapsedMs);
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Update(elapsedMs);
			break;
		}
	}
	break;

	case CGoomba::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Stop();
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Stop();
			break;
		}
	}
	NextAction();
	break;
	}
}

void CGoomba::Jump(float elapsedMs)
{
	switch (_actionStage)
	{
	case CGoomba::EActionStage::START:
	{
		if (_wing)
		{
			_animations[ANI_GOOMBA_WING_FLAP]->Play(true);
		}

		_vy = JUMP_FORCE;
		_ground = false;

		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Play(true);
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Play(true);
			break;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CGoomba::EActionStage::PROGRESS:
	{
		if (!_left)
		{
			_vx = -WALK_SPEED;
		}
		else
		{
			_vx = WALK_SPEED;
		}

		if (_wing)
		{
			_animations[ANI_GOOMBA_WING_FLAP]->Update(elapsedMs);
		}

		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Update(elapsedMs);
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Update(elapsedMs);
			break;
		}

		if (_ground)
		{
			SetNextAction(EAction::WALK);
		}
	}
	break;

	case CGoomba::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CGoomba::EType::GOOMBA:
			_animations[ANI_GOOMBA_WALK]->Stop();
			break;
		case CGoomba::EType::RED_GOOMBA:
			_animations[ANI_RED_GOOMBA_WALK]->Stop();
			break;
		}
	}
	NextAction();
	break;
	}
}

void CGoomba::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CGoomba::EActionStage::START:
	{
		_currentDecayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CGoomba::EActionStage::PROGRESS:
	{
		if (_currentDecayInterval > 0)
		{
			_currentDecayInterval -= elapsedMs;
		}
		else
		{
			_start = false;
			Deactive();
		}
	}
	break;

	case CGoomba::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CGoomba::Thrown(float elapsedMs)
{
	switch (_actionStage)
	{
	case CGoomba::EActionStage::START:
	{
		_vy = JUMP_FORCE;
		_currentDecayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CGoomba::EActionStage::PROGRESS:
	{
		if (_currentDecayInterval > 0)
		{
			_currentDecayInterval -= elapsedMs;
		}
		else
		{
			_start = false;
			Deactive();
		}
	}
	break;

	case CGoomba::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CGoomba::IsCollidable()
{
	switch (_action)
	{
	case CGoomba::EAction::WALK:
	case CGoomba::EAction::JUMP:
		return 1;
		break;

	case CGoomba::EAction::DIE:
	case CGoomba::EAction::THROWN:
		return 0;
		break;
	}
	return 0;
}

int CGoomba::IsBlocking()
{
	return 0;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CGoomba::EAction::WALK:
	case CGoomba::EAction::JUMP:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CGoomba::EAction::DIE:
	case CGoomba::EAction::THROWN:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CGoomba::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CGoomba::OnCollisionWith(pCollision e)
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
		if (e->_ny > 0 && !e->_target->IsBlocking())
		{
			float top = 0;
			float temp = 0;
			e->_target->GetBoundingBox(temp, top, temp, temp);
			_y = top + BLOCK_PUSH_FACTOR;
			_vy = 0;
			if (e->_ny > 0) _ground = true;
		}
	}
}
