#include "Venus.h"
#include "../../SuperMarioBros3.h"

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
	STAND_INTERVAL = statsNode.attribute("SHOOT_INTERVAL").as_float();
	GROW_LIMIT = statsNode.attribute("GROW_LIMIT").as_float();
	BURROW_LIMIT = _y;
	SPEED = statsNode.attribute("SPEED").as_float();
	FIREBALL_VX = statsNode.attribute("FIREBALL_VX").as_float();
	FIREBALL_VY = statsNode.attribute("FIREBALL_VY").as_float();

	/* Logic */
	_targetName = statsNode.attribute("target").as_string();
	_type = EType(statsNode.attribute("type").as_uint());

	/* Sensor */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("Sensor");
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
	SetAction(EAction::STAND);
}

void CVenus::Update(float elapsedMs)
{
	if (!_start) Start();

	UpdateSensor();
	AcquireTarget();

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
	case CVenus::EAction::BURROW:
	case CVenus::EAction::GROW:
	{
		switch (_type)
		{
		case CVenus::EType::VENUS:
			_animations[ANI_PIRANHA]->Render(_x, _y);
			break;
		case CVenus::EType::RED_VENUS:
			_animations[ANI_RED_PIRANHA]->Render(_x, _y);
			break;
		}
	}
	break;

	case CVenus::EAction::STAND:
		switch (_type)
		{
		case CVenus::EType::VENUS:
			if (_left)
			{
				if (_up) _sprites[SPR_VENUS_UP1_LEFT]->Render(_x, _y);
				else _sprites[SPR_VENUS_DOWN1_LEFT]->Render(_x, _y);
			}
			else
			{
				if (_up) _sprites[SPR_VENUS_UP1_RIGHT]->Render(_x, _y);
				else _sprites[SPR_VENUS_DOWN1_RIGHT]->Render(_x, _y);
			}
			break;

		case CVenus::EType::RED_VENUS:
			if (_left)
			{
				if (_up) _sprites[SPR_RED_VENUS_UP1_LEFT]->Render(_x, _y);
				else _sprites[SPR_RED_VENUS_DOWN1_LEFT]->Render(_x, _y);
			}
			else
			{
				if (_up) _sprites[SPR_RED_VENUS_UP1_RIGHT]->Render(_x, _y);
				else _sprites[SPR_RED_VENUS_DOWN1_RIGHT]->Render(_x, _y);
			}
			break;
		}
		break;

	case CVenus::EAction::SHOOT:
		switch (_type)
		{
		case CVenus::EType::VENUS:
			if (_left)
			{
				if (_up) _sprites[SPR_VENUS_UP2_LEFT]->Render(_x, _y);
				else _sprites[SPR_VENUS_DOWN2_LEFT]->Render(_x, _y);
			}
			else
			{
				if (_up) _sprites[SPR_VENUS_UP2_RIGHT]->Render(_x, _y);
				else _sprites[SPR_VENUS_DOWN2_RIGHT]->Render(_x, _y);
			}
			break;

		case CVenus::EType::RED_VENUS:
			if (_left)
			{
				if (_up) _sprites[SPR_RED_VENUS_UP2_LEFT]->Render(_x, _y);
				else _sprites[SPR_RED_VENUS_DOWN2_LEFT]->Render(_x, _y);
			}
			else
			{
				if (_up) _sprites[SPR_RED_VENUS_UP2_RIGHT]->Render(_x, _y);
				else _sprites[SPR_RED_VENUS_DOWN2_RIGHT]->Render(_x, _y);
			}
			break;
		}
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
		Hide(elapsedMs);
		break;

	case CVenus::EAction::GROW:
		Grow(elapsedMs);
		break;

	case CVenus::EAction::STAND:
		Stand(elapsedMs);
		break;

	case CVenus::EAction::BURROW:
		Burrow(elapsedMs);
		break;

	case CVenus::EAction::SHOOT:
		Shoot(elapsedMs);
		break;

	case CVenus::EAction::DIE:
		Die(elapsedMs);
		break;
	}
}

void CVenus::Hide(float elapsedMs)
{
	switch (_actionStage)
	{
	case CVenus::EActionStage::START:
	{
		_hideInterval = HIDE_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
		if (!_sensor->_activate)
		{
			if (_hideInterval > 0) _hideInterval -= elapsedMs;
			else SetNextAction(EAction::GROW);
		}
		else _hideInterval = HIDE_INTERVAL;
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
		/* Animation Start */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Play(true);
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Play(true);
				break;
			}
		}

		_growLimit = _y + GROW_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Update(elapsedMs);
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Update(elapsedMs);
				break;
			}
		}

		if (_y < _growLimit) _y += SPEED * elapsedMs;
		else
		{
			_y = _growLimit;
			SetNextAction(EAction::STAND);
		}
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Stop();
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Stop();
				break;
			}
		}

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
		_standInterval = STAND_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
		if (_standInterval > 0) _standInterval -= elapsedMs;
		else SetNextAction(EAction::SHOOT);
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
		/* Animation Start */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Play(true);
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Play(true);
				break;
			}
		}

		_burrowLimit = BURROW_LIMIT;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Update(elapsedMs);
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Update(elapsedMs);
				break;
			}
		}

		if (_y > _burrowLimit) _y += -SPEED * elapsedMs;
		else
		{
			_y = _burrowLimit;
			SetNextAction(EAction::HIDE);
		}
	}
	break;

	case CVenus::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			switch (_type)
			{
			case CVenus::EType::VENUS:
				_animations[ANI_PIRANHA]->Stop();
				break;

			case CVenus::EType::RED_VENUS:
				_animations[ANI_RED_PIRANHA]->Stop();
				break;
			}
		}
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
		ShootFireball();
		_shootInterval = SHOOT_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CVenus::EActionStage::PROGRESS:
	{
		if (_standInterval > 0) _standInterval -= elapsedMs;
		else SetNextAction(EAction::BURROW);
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
		_sensor->Destroy();
		Destroy();
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

void CVenus::UpdateSensor()
{
	if (_sensor != nullptr)
	{
		_sensor->SetPosition(_x, _y);
	}
}

void CVenus::AcquireTarget()
{
	if (_game->Get(_targetName) != nullptr)
	{
		if (_x > _game->Get(_targetName)->_x) _left = true;
		else _left = false;

		if (_y < _game->Get(_targetName)->_y) _up = true;
		else _up = false;
	}
}

void CVenus::ShootFireball()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Fireball */
	pugi::xml_node sensorNode = prefab.child("Prefab").child("Fireball");
	std::string fireballName = _name + sensorNode.attribute("name").as_string();
	if (_game->Get(fireballName) == nullptr)
	{
		auto fireball = _game->Create(
			_scene,
			sensorNode.attribute("actor").as_uint(),
			fireballName,
			sensorNode.attribute("source").as_string(),
			_x, _y + SHOT_OFFSET, _gridX, _gridY, _layer, _active
		);

		if (_left) fireball->_vx = -FIREBALL_VX;
		else fireball->_vx = FIREBALL_VX;

		if (_up) fireball->_vy = FIREBALL_VY;
		else fireball->_vy = -FIREBALL_VY;
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
