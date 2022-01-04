#include "Leaf.h"
#include "../../SuperMarioBros3.h"

void CLeaf::Load()
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
	MOVE_SPEED = statsNode.attribute("MOVE_SPEED").as_float();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	REVERSE_INTERVAL = statsNode.attribute("REVERSE_INTERVAL").as_float();
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
	SPAWN_LIMIT = statsNode.attribute("SPAWN_LIMIT").as_float();
}

void CLeaf::Start()
{
	_start = true;
	_left = false;
	_vx = 0;
	_vy = 0;
	_ax = 0;
	_ay = 0;
	SetAction(EAction::SPAWN);
}

void CLeaf::Update(float elapsedMs)
{
	if (!_start) Start();

	if (_action != EAction::SPAWN)
	{
		_ay = -GRAVITY;
	}
	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;

	HandleAction(elapsedMs);
}

void CLeaf::Render()
{
	switch (_action)
	{
	case CLeaf::EAction::SPAWN:
	case CLeaf::EAction::MOVE:
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		if (_left) _sprites[SPR_LEAF_LEFT]->Render(_x, _y);
		else _sprites[SPR_LEAF_RIGHT]->Render(_x, _y);
		break;
	case CLeaf::EAction::CONSUMED:
		break;
	}
}

void CLeaf::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CLeaf::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CLeaf::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CLeaf::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CLeaf::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CLeaf::EActionStage::START:
	{
		_vy = MOVE_SPEED;
		_spawnLimit = SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CLeaf::EActionStage::PROGRESS:
	{
		if (_spawnLimit > 0)
		{
			_vy = MOVE_SPEED;
			_spawnLimit -= elapsedMs;
		}
		else
		{
			SetNextAction(EAction::MOVE);
		}
	}
	break;

	case CLeaf::EActionStage::EXIT:
	{
		_vy = 0;
	}
	NextAction();
	break;
	}
}

void CLeaf::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CLeaf::EActionStage::START:
	{
		_reverseInterval = REVERSE_INTERVAL;
		_decayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CLeaf::EActionStage::PROGRESS:
	{
		if (_reverseInterval > 0)
		{
			_reverseInterval -= elapsedMs;
		}
		else
		{
			_left = !_left;
			_reverseInterval = REVERSE_INTERVAL;
		}

		if (_left) _vx = -MOVE_SPEED;
		else _vx = MOVE_SPEED;

		if (_decayInterval > 0)
		{
			_decayInterval -= elapsedMs;
		}
		else
		{
			Destroy();
		}
	}
	break;

	case CLeaf::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CLeaf::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CLeaf::EActionStage::START:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CLeaf::EActionStage::PROGRESS:
	{
	}
	break;

	case CLeaf::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CLeaf::IsCollidable()
{
	switch (_action)
	{
	case CLeaf::EAction::MOVE:
		return 1;
		break;
	case CLeaf::EAction::CONSUMED:
	case CLeaf::EAction::SPAWN:
		return 0;
		break;
	}
	return 0;
}

int CLeaf::IsBlocking()
{
	return 0;
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CLeaf::EAction::MOVE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CLeaf::EAction::CONSUMED:
	case CLeaf::EAction::SPAWN:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CLeaf::OnNoCollision(float elapsedMs)
{
}

void CLeaf::OnCollisionWith(pCollision e)
{
}
