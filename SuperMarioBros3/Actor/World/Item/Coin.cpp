#include "Coin.h"
#include "../../../Engine/Game.h"
#include "../../../Engine/Library/pugixml.hpp"

void CCoin::Load()
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
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
	SPEED = statsNode.attribute("SPEED").as_float();
}

void CCoin::Start()
{
	_start = true;
}

void CCoin::Update(float elapsedMs)
{
	if (!_start) Start();
	HandleAction(elapsedMs);
}

void CCoin::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CCoin::EAction::IDLE:
		_sprites[SPR_COIN1]->Render(_x, _y);
		break;

	case CCoin::EAction::CONSUMED:
		_animations[ANI_COIN]->Render(_x, _y);
		break;
	}
}

void CCoin::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CCoin::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CCoin::EAction::CONSUMED:
		Consumed(elapsedMs);
		break;
	}
}

void CCoin::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CCoin::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CCoin::EActionStage::PROGRESS:
	{
	}
	break;

	case CCoin::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CCoin::Consumed(float elapsedMs)
{
	switch (_actionStage)
	{
	case CCoin::EActionStage::START:
	{
		_animations[ANI_COIN]->Play(true);
		_currentDecayInterval = DECAY_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CCoin::EActionStage::PROGRESS:
	{
		if (_currentDecayInterval > 0)
		{
			_currentDecayInterval -= elapsedMs;
			_y += SPEED * elapsedMs;
			_animations[ANI_COIN]->Update(elapsedMs);
		}
		else
		{
			Destroy();
		}

	}
	break;

	case CCoin::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CCoin::IsCollidable()
{
	switch (_action)
	{
	case CCoin::EAction::IDLE:
		return 1;
		break;

	case CCoin::EAction::CONSUMED:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CCoin::IsBlocking()
{
	return 0;
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CCoin::EAction::IDLE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;
	case CCoin::EAction::CONSUMED:
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
		break;
	}
}

void CCoin::OnNoCollision(float elapsedMs)
{
}

void CCoin::OnCollisionWith(pCollision e)
{
}
