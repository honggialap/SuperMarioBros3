#include "Mushroom.h"
#include "../../../Actor/World/Prop/Platform/HollowedPlatform.h"
#include "../../../Engine/Game.h"
#include "../../../Engine/Library/pugixml.hpp"

void CMushroom::Load()
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
	SPAWN_LIMIT = statsNode.attribute("SPAWN_LIMIT").as_float();
}

void CMushroom::Start()
{
	_start = true;
	_left = false;
	_vx = 0;
	_vy = 0;
	_ax = 0;
	_ay = 0;
	SetAction(EAction::SPAWN);
}

void CMushroom::Update(float elapsedMs)
{
	if (!_start) Start();

	if (_action != EAction::SPAWN)
	{
		_ay = -GRAVITY;
	}
	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CMushroom::Render()
{
	switch (_action)
	{
	case CMushroom::EAction::SPAWN:
	case CMushroom::EAction::MOVE:
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		_sprites[SPR_MUSHROOM]->Render(_x, _y);
		break;
	case CMushroom::EAction::CONSUMED:
		break;
	}
}

void CMushroom::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CMushroom::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CMushroom::EAction::MOVE:
		Move(elapsedMs);
		break;

	case CMushroom::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CMushroom::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMushroom::EActionStage::START:
	{
		_vy = MOVE_SPEED;
		_spawnLimit = SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMushroom::EActionStage::PROGRESS:
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

	case CMushroom::EActionStage::EXIT:
	{
		_ay = 0;
		_vy = 0;
	}
	NextAction();
	break;
	}
}

void CMushroom::Move(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMushroom::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMushroom::EActionStage::PROGRESS:
	{
		if (_left) _vx = -MOVE_SPEED;
		else _vx = MOVE_SPEED;
	}
	break;

	case CMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMushroom::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMushroom::EActionStage::START:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMushroom::EActionStage::PROGRESS:
	{
	}
	break;

	case CMushroom::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CMushroom::IsCollidable()
{
	switch (_action)
	{
	case CMushroom::EAction::MOVE:
		return 1;
		break;
	case CMushroom::EAction::CONSUMED:
	case CMushroom::EAction::SPAWN:
		return 0;
		break;
	}
	return 0;
}

int CMushroom::IsBlocking()
{
	return 0;
}

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CMushroom::EAction::MOVE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CMushroom::EAction::CONSUMED:
	case CMushroom::EAction::SPAWN:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CMushroom::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CMushroom::OnCollisionWith(pCollision e)
{
	if (e->_ny != 0 && e->_target->IsBlocking())
	{
		_vy = 0;
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
		}
	}
}
