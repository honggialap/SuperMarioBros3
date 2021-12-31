#include "Brick.h"
#include "../../../../Engine/Game.h"
#include "../../../../Engine/Library/pugixml.hpp"

void CBrick::Load()
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
	BOUNCE_LIMIT = statsNode.attribute("BOUNCE_LIMIT").as_float();
}

void CBrick::Start()
{
	_start = true;
	_vx = 0;
	_vy = 0;
	_ax = 0;
	_ay = 0;
	SetAction(EAction::IDLE);
}

void CBrick::Update(float elapsedMs)
{
	if (!_start) Start();
	HandleAction(elapsedMs);
}

void CBrick::Render()
{
	switch (_action)
	{
	case CBrick::EAction::IDLE:
	case CBrick::EAction::BOUNCE:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		_animations[ANI_BRICK]->Render(_x, _y);
	}
		break;
	case CBrick::EAction::BROKE:
		break;
	}
}

void CBrick::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CBrick::EAction::IDLE:
		Idle(elapsedMs);
		break;
	case CBrick::EAction::BOUNCE:
		Bounce(elapsedMs);
		break;
	case CBrick::EAction::BROKE:
		Broke(elapsedMs);
		break;
	}
}

void CBrick::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBrick::EActionStage::START:
	{
		_animations[ANI_BRICK]->Play(true);
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBrick::EActionStage::PROGRESS:
	{
		_animations[ANI_BRICK]->Update(elapsedMs);
	}
	break;

	case CBrick::EActionStage::EXIT:
	{
		_animations[ANI_BRICK]->Stop();
	}
	NextAction();
	break;
	}
}

void CBrick::Bounce(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBrick::EActionStage::START:
	{
		_originY = _y;
		_return = false;
		_animations[ANI_BRICK]->Play(true);
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBrick::EActionStage::PROGRESS:
	{
		_animations[ANI_BRICK]->Update(elapsedMs);

		if (!_return)
		{

			if (_y < _originY + BOUNCE_LIMIT && !_return)
			{
				_y += MOVE_SPEED * elapsedMs;
			}
			else
			{
				_return = true;
			}
		}
		else
		{
			if (_y > _originY && _return)
			{
				_y -= MOVE_SPEED * elapsedMs;
			}
			else
			{
				_return = false;
				_y = _originY;
				SetNextAction(EAction::IDLE);
			}
		}
	}
	break;

	case CBrick::EActionStage::EXIT:
	{
		_animations[ANI_BRICK]->Stop();
	}
	NextAction();
	break;
	}
}

void CBrick::Broke(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBrick::EActionStage::START:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBrick::EActionStage::PROGRESS:
	{
	}
	break;

	case CBrick::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CBrick::IsCollidable()
{
	switch (_action)
	{
	case CBrick::EAction::IDLE:
	case CBrick::EAction::BOUNCE:
		return 1;
		break;
	case CBrick::EAction::BROKE:
		return 0;
		break;
	}
}

int CBrick::IsBlocking()
{
	switch (_action)
	{
	case CBrick::EAction::IDLE:
	case CBrick::EAction::BOUNCE:
		return 1;
		break;
	case CBrick::EAction::BROKE:
		return 0;
		break;
	}
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CBrick::EAction::IDLE:
	case CBrick::EAction::BOUNCE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CBrick::EAction::BROKE:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CBrick::OnNoCollision(float elapsedMs)
{
}

void CBrick::OnCollisionWith(pCollision e)
{
}
