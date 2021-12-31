#include "Koopa.h"
#include "Engine/Game.h"
#include "Engine/Library/pugixml.hpp"
#include "SuperMarioBros3.h"

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
	JUMP_FORCE = statsNode.attribute("JUMP_FORCE").as_float();
	JUMP_INTERVAL = statsNode.attribute("JUMP_INTERVAL").as_float();
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
}

void CKoopa::Start()
{
	_start = true;
	_left = true;
	_ax = 0.0f;
	_ay = 0.0f;
	_sensor->Active();
	SetAction(EAction::WALK);
}

void CKoopa::Update(float elapsedMs)
{
	if (!_start) Start();

	_ay = -GRAVITY;
	_vx += _ax * elapsedMs;
	_vy += _ay * elapsedMs;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
	HandleAction(elapsedMs);
}

void CKoopa::Render()
{
	switch (_action)
	{
	case CKoopa::EAction::WALK:
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
			if (_left) _sprites[SPR_KOOPA_WING1_LEFT]->Render(_x, _y);
			else _sprites[SPR_KOOPA_WING1_RIGHT]->Render(_x, _y);
		}
	}
		break;
	case CKoopa::EAction::JUMP:
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
			_animations[ANI_KOOPA_BLINK]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK]->Render(_x, _y);
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
			_animations[ANI_KOOPA_SPIN]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN]->Render(_x, _y);
			break;
		}
	}
		break;
	case CKoopa::EAction::DIE:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_sprites[SPR_KOOPA_RETRACT1]->Render(_x, _y);
			break;
		case CKoopa::EType::RED_KOOPA:
			_sprites[SPR_RED_KOOPA_RETRACT1]->Render(_x, _y);
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
	case CKoopa::EAction::WALK:
		Walk(elapsedMs);
		break;
	case CKoopa::EAction::JUMP:
		Jump(elapsedMs);
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

void CKoopa::Walk(float elapsedMs)
{
	switch (_actionStage)
	{
	case CKoopa::EActionStage::START:
	{
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
		}	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
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
	}
	NextAction();
	break;
	}
}

void CKoopa::Jump(float elapsedMs)
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
		}	}
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
			_animations[ANI_KOOPA_BLINK]->Play(true);
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK]->Play(true);
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
			_animations[ANI_KOOPA_BLINK]->Update(elapsedMs);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK]->Update(elapsedMs);
			break;
		}	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_BLINK]->Stop();
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_BLINK]->Stop();
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
			_animations[ANI_KOOPA_SPIN]->Play(true);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN]->Play(true);
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
			_animations[ANI_KOOPA_SPIN]->Update(elapsedMs);
			break;

		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN]->Update(elapsedMs);
			break;
		}	
	}
	break;

	case CKoopa::EActionStage::EXIT:
	{
		switch (_type)
		{
		case CKoopa::EType::KOOPA:
			_animations[ANI_KOOPA_SPIN]->Stop();
			break;
		case CKoopa::EType::RED_KOOPA:
			_animations[ANI_RED_KOOPA_SPIN]->Stop();
			break;
		}
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
		_currentDecayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CKoopa::EActionStage::PROGRESS:
	{
		if (_currentDecayInterval > 0)
		{
			_currentDecayInterval -= elapsedMs;
		}
		else
		{
			_start = false;
			_sensor->Deactive();
			Deactive();
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
	return 0;
}

int CKoopa::IsBlocking()
{
	return 0;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CKoopa::OnNoCollision(float elapsedMs)
{
}

void CKoopa::OnCollisionWith(pCollision e)
{
}
