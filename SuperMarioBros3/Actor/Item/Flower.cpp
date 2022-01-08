#include "Flower.h"
#include "../../SuperMarioBros3.h"
#include "../Prop/Platform.h"

void CFlower::Load()
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
	SPAWN_LIMIT = statsNode.attribute("SPAWN_LIMIT").as_float();
}

void CFlower::Start()
{
	_start = true;
	_left = false;
	SetAction(EAction::SPAWN);
}

void CFlower::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CFlower::Render()
{
	switch (_action)
	{
	case CFlower::EAction::SPAWN:
	case CFlower::EAction::IDLE:
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		_sprites[SPR_FLOWER]->Render(_x, _y);
		break;
	case CFlower::EAction::CONSUMED:
		break;
	}
}

void CFlower::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CFlower::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CFlower::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CFlower::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CFlower::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CFlower::EActionStage::START:
	{
		_spawnLimit = _y + SPAWN_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CFlower::EActionStage::PROGRESS:
	{
		if (_y < _spawnLimit)
		{
			_y += MOVE_SPEED * elapsedMs;
		}
		else
		{
			_y = _spawnLimit;
			SetNextAction(EAction::IDLE);
		}
	}
	break;

	case CFlower::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CFlower::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CFlower::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CFlower::EActionStage::PROGRESS:
	{
	}
	break;

	case CFlower::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}

}

void CFlower::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CFlower::EActionStage::START:
	{
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CFlower::EActionStage::PROGRESS:
	{
	}
	break;

	case CFlower::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}

}

int CFlower::IsCollidable()
{
	switch (_action)
	{
	case CFlower::EAction::IDLE:
		return 1;
		break;
	case CFlower::EAction::CONSUMED:
	case CFlower::EAction::SPAWN:
		return 0;
		break;
	}
	return 0;
}

int CFlower::IsBlocking()
{
	return 0;
}

void CFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CFlower::EAction::IDLE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CFlower::EAction::CONSUMED:
	case CFlower::EAction::SPAWN:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CFlower::OnNoCollision(float elapsedMs)
{
}

void CFlower::OnCollisionWith(pCollision e)
{
}
