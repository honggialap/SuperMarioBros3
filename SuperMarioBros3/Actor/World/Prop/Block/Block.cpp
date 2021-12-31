#include "Block.h"
#include "SuperMarioBros3.h"
#include "Actor/World/Item/Coin.h"
#include "Engine/Game.h"
#include "Engine/Library/pugixml.hpp"

void CBlock::Load()
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
	_type = EType(statsNode.attribute("TYPE").as_int());
	_spawn = ESpawn(statsNode.attribute("SPAWN").as_int());
	_target = dynamic_cast<pMario>(_game->Get(statsNode.attribute("targetName").as_string()));
}

void CBlock::Start()
{
	_start = true;
	_vx = 0;
	_vy = 0;
	_ax = 0;
	_ay = 0;
	SetAction(EAction::IDLE);
}

void CBlock::Update(float elapsedMs)
{
	if (!_start) Start();
	HandleAction(elapsedMs);
}

void CBlock::Render()
{
	switch (_action)
	{

	case CBlock::EAction::IDLE:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_BLOCK_QUESTION]->Render(_x, _y);
			break;
		case CBlock::EType::HIDDEN:
			_animations[ANI_BRICK]->Render(_x, _y);
			break;
		case CBlock::EType::INVISIBLE:
			break;
		}
	}
		break;

	case CBlock::EAction::SPAWN:
	case CBlock::EAction::EMTPY:
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		_sprites[SPR_BLOCK_EMPTY]->Render(_x, _y);
	}
		break;
	}
}

void CBlock::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CBlock::EAction::IDLE:
		Idle(elapsedMs);
		break;
	case CBlock::EAction::SPAWN:
		Spawn(elapsedMs);
		break;
	case CBlock::EAction::EMTPY:
		Empty(elapsedMs);
		break;
	}
}

void CBlock::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::START:
	{
		_animations[ANI_BLOCK_QUESTION]->Play(true);
		_animations[ANI_BRICK]->Play(true);
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
		_animations[ANI_BLOCK_QUESTION]->Update(elapsedMs);
		_animations[ANI_BRICK]->Update(elapsedMs);
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
		_animations[ANI_BLOCK_QUESTION]->Stop();
		_animations[ANI_BRICK]->Stop();
	}
	NextAction();
	break;
	}
}

void CBlock::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::START:
	{
		_return = false;
		if (_topBounce)
		{
			_origin = _y;
		}
		else
		{
			_origin = _x;
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
		if (_topBounce)
		{
			if (!_return)
			{
				if (_y < _origin + BOUNCE_LIMIT)
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
				if (_y > _origin)
				{
					_y -= MOVE_SPEED * elapsedMs;
				}
				else
				{
					_return = false;
					_y = _origin;
					SpawnItem();
					SetNextAction(EAction::EMTPY);
				}
			}
		}
		else
		{
			if (_leftBounce)
			{
				if (!_return)
				{
					if (_x > _origin - BOUNCE_LIMIT && !_return)
					{
						_x -= MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = true;
					}
				}
				else
				{
					if (_x < _origin && _return)
					{
						_x += MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = false;
						_x = _origin;
						SpawnItem();
						SetNextAction(EAction::EMTPY);
					}
				}
			}
			else
			{
				if (!_return)
				{

					if (_x < _origin + BOUNCE_LIMIT && !_return)
					{
						_x += MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = true;
					}
				}
				else
				{
					if (_x > _origin && _return)
					{
						_x -= MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = false;
						_x = _origin;
						SpawnItem();
						SetNextAction(EAction::EMTPY);
					}
				}
			}
		}
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CBlock::Empty(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CBlock::SpawnItem()
{
	unsigned int spawnId = 0;
	switch (_spawn)
	{
	case CBlock::ESpawn::COIN:
		spawnId = 0;
		break;

	case CBlock::ESpawn::POWER_UP:
		spawnId = 2;
		if (_target != nullptr)
		{
			if (_target->_power != CMario::EPower::SMALL)
			{
				spawnId = 1;
			}
		}
		break;

	case CBlock::ESpawn::EXTRA_LIFE:
		spawnId = 3;
		break;

	case CBlock::ESpawn::SWITCH:
		spawnId = 4;
		break;
	}

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());
	for (auto gameObjectNode = prefab.child("Prefab").child("GameObject");
		gameObjectNode;
		gameObjectNode = gameObjectNode.next_sibling("GameObject"))
	{
		if (spawnId == gameObjectNode.attribute("id").as_uint())
		{
			auto gameObject = _game->Create(
				_scene,
				gameObjectNode.attribute("actor").as_uint(),
				_name.append(gameObjectNode.attribute("name").as_string()),
				gameObjectNode.attribute("source").as_string(),
				_x, _y, _gridX, _gridY, _layer, _active
			);
			if (dynamic_cast<pCoin>(gameObject))
			{
				dynamic_cast<pCoin>(gameObject)->SetAction(CCoin::EAction::CONSUMED);
			}
		}
	}

}

int CBlock::IsCollidable()
{
	return 1;
}

int CBlock::IsBlocking()
{
	return 1;
}

void CBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CBlock::OnNoCollision(float elapsedMs)
{
}

void CBlock::OnCollisionWith(pCollision e)
{
}
