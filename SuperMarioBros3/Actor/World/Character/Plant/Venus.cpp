#include "Venus.h"
#include "../../../../Engine/Game.h"
#include "../../../../Engine/Library/pugixml.hpp"

void CVenus::Load()
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
	SHOOT_INTERVAL = statsNode.attribute("SHOOT_INTERVAL").as_float();
	GROW_LIMIT = statsNode.attribute("GROW_LIMIT").as_float();
	BORROW_LIMIT = _y;

	/* Fireball */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("GameObject");
	_fireball = dynamic_cast<pVenusFireball>(
		_game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			_name.append(sensorNode.attribute("name").as_string()),
			sensorNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		)
	);

	/* Sensor */
	sensorNode = sensorNode.next_sibling("GameObject");
	_sensor = dynamic_cast<pVenusSensor>(
		_game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			_name.append(sensorNode.attribute("name").as_string()),
			sensorNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		)
	);
}

void CVenus::Start()
{
	_start = true;
	_ax = 0.0f;
	_ay = 0.0f;
	_sensor->Active();
	_fireball->Active();
	SetAction(EAction::STAND);
}

void CVenus::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CVenus::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CVenus::EAction::HIDE:
		break;
	case CVenus::EAction::GROW:
		_animations[ANI_PIRANHA]->Render(_x, _y);
		break;
	case CVenus::EAction::STAND:
		_sprites[SPR_PIRANHA1]->Render(_x, _y);
		break;
	case CVenus::EAction::BURROW:
		_animations[ANI_PIRANHA]->Render(_x, _y);
		break;
	case CVenus::EAction::SHOOT:
		_animations[ANI_PIRANHA]->Render(_x, _y);
		break;
	case CVenus::EAction::DIE:
		break;
	}

}

void CVenus::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CVenus::EAction::HIDE:
		break;
	case CVenus::EAction::GROW:
		break;
	case CVenus::EAction::STAND:
		break;
	case CVenus::EAction::BURROW:
		break;
	case CVenus::EAction::SHOOT:
		break;
	case CVenus::EAction::DIE:
		break;
	}
}

void CVenus::Hide(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CVenus::Grow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CVenus::Stand(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CVenus::Burrow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CVenus::Shoot(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CVenus::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CVenus::IsCollidable()
{
	return 0;
}

int CVenus::IsBlocking()
{
	return 0;
}

void CVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CVenus::OnNoCollision(float elapsedMs)
{
}

void CVenus::OnCollisionWith(pCollision e)
{
}
