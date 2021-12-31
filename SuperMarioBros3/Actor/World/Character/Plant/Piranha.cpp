#include "Piranha.h"
#include "Engine/Game.h"
#include "Engine/Library/pugixml.hpp"

void CPiranha::Load()
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
	HIDE_INTERVAL = statsNode.attribute("HIDE_INTERVAL").as_float();
	STAND_INTERVAL = statsNode.attribute("STAND_INTERVAL").as_float();
	GROW_LIMIT = statsNode.attribute("GROW_LIMIT").as_float();
	BORROW_LIMIT = _y;

	/* Sensor */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("GameObject");
	_sensor = dynamic_cast<pPiranhaSensor>(
		_game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			_name.append(sensorNode.attribute("name").as_string()),
			sensorNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		)
	);
}

void CPiranha::Start()
{
	_start = true;
	_ax = 0.0f;
	_ay = 0.0f;
	_sensor->Active();
	SetAction(EAction::STAND);
}

void CPiranha::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CPiranha::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CPiranha::EAction::HIDE:
		break;
	case CPiranha::EAction::GROW:
		_animations[ANI_PIRANHA]->Render(_x, _y);
		break;
	case CPiranha::EAction::STAND:
		_sprites[SPR_PIRANHA1]->Render(_x, _y);
		break;
	case CPiranha::EAction::BURROW:
		_animations[ANI_PIRANHA]->Render(_x, _y);
		break;
	case CPiranha::EAction::DIE:
		break;
	}
}

void CPiranha::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CPiranha::EAction::HIDE:
		Hide(elapsedMs);
		break;
	case CPiranha::EAction::GROW:
		Grow(elapsedMs);
		break;
	case CPiranha::EAction::STAND:
		Stand(elapsedMs);
		break;
	case CPiranha::EAction::BURROW:
		Burrow(elapsedMs);
		break;
	case CPiranha::EAction::DIE:
		Die(elapsedMs);
		break;
	}
}

void CPiranha::Hide(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPiranha::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPiranha::EActionStage::PROGRESS:
	{
	}
	break;

	case CPiranha::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CPiranha::Grow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPiranha::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPiranha::EActionStage::PROGRESS:
	{
	}
	break;

	case CPiranha::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CPiranha::Stand(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPiranha::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPiranha::EActionStage::PROGRESS:
	{
	}
	break;

	case CPiranha::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CPiranha::Burrow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPiranha::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPiranha::EActionStage::PROGRESS:
	{
	}
	break;

	case CPiranha::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CPiranha::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPiranha::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPiranha::EActionStage::PROGRESS:
	{
	}
	break;

	case CPiranha::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CPiranha::IsCollidable()
{
	return 0;
}

int CPiranha::IsBlocking()
{
	return 0;
}

void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CPiranha::OnNoCollision(float elapsedMs)
{
}

void CPiranha::OnCollisionWith(pCollision e)
{
}
