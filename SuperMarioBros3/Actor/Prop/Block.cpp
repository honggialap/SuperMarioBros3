#include "Block.h"
#include "../../SuperMarioBros3.h"
#include "../Item/Coin.h"

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
	{
		if (_renderBody) _sprites[BBOX]->Render(_x, _y);
		_sprites[SPR_BLOCK_EMPTY]->Render(_renderX, _renderY);
	}
	break;
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
		_renderX = _x;
		_renderY = _y;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
		if (_topBounce)
		{
			if (!_return)
			{
				if (_renderY < _origin + BOUNCE_LIMIT)
				{
					_renderY += MOVE_SPEED * elapsedMs;
				}
				else
				{
					_return = true;
				}
			}
			else
			{
				if (_renderY > _origin)
				{
					_renderY -= MOVE_SPEED * elapsedMs;
				}
				else
				{
					_return = false;
					_renderY = _origin;
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
					if (_renderX > _origin - BOUNCE_LIMIT && !_return)
					{
						_renderX -= MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = true;
					}
				}
				else
				{
					if (_renderX < _origin && _return)
					{
						_renderX += MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = false;
						_renderX = _origin;
						SpawnItem();
						SetNextAction(EAction::EMTPY);
					}
				}
			}
			else
			{
				if (!_return)
				{

					if (_renderX < _origin + BOUNCE_LIMIT && !_return)
					{
						_renderX += MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = true;
					}
				}
				else
				{
					if (_renderX > _origin && _return)
					{
						_renderX -= MOVE_SPEED * elapsedMs;
					}
					else
					{
						_return = false;
						_renderX = _origin;
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
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	switch (_spawn)
	{
	case CBlock::ESpawn::COIN:
	{
		auto coinNode = prefab.child("Prefab").child("Coin");
		std::string coinName = _name + coinNode.attribute("name").as_string();
		auto gameObject = _game->Create(
			_scene,
			coinNode.attribute("actor").as_uint(),
			coinName,
			coinNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		);
		dynamic_cast<pCoin>(gameObject)->SetAction(CCoin::EAction::CONSUMED);
	}
		break;

	case CBlock::ESpawn::POWER_UP:
	{
		if (_target != nullptr)
		{
			if (_target->_power != CMario::EPower::SMALL)
			{
				auto leafNode = prefab.child("Prefab").child("Leaf");
				std::string leafName = _name + leafNode.attribute("name").as_string();
				auto gameObject = _game->Create(
					_scene,
					leafNode.attribute("actor").as_uint(),
					leafName,
					leafNode.attribute("source").as_string(),
					_x, _y, _gridX, _gridY, _layer, _active
				);
			}
			else
			{
				auto mushroomNode = prefab.child("Prefab").child("Mushroom");
				std::string mushroomName = _name + mushroomNode.attribute("name").as_string();
				auto gameObject = _game->Create(
					_scene,
					mushroomNode.attribute("actor").as_uint(),
					mushroomName,
					mushroomNode.attribute("source").as_string(),
					_x, _y, _gridX, _gridY, _layer, _active
				);
			}
		}
	}
		break;

	case CBlock::ESpawn::EXTRA_LIFE:
	{
		auto extraLifeNode = prefab.child("Prefab").child("ExtraLife");
		std::string extraLifeName = _name + extraLifeNode.attribute("name").as_string();
		auto gameObject = _game->Create(
			_scene,
			extraLifeNode.attribute("actor").as_uint(),
			extraLifeName,
			extraLifeNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		);
	}
		break;

	case CBlock::ESpawn::SWITCH:
	{
		auto switchNode = prefab.child("Prefab").child("Switch");
		std::string switchName = _name + switchNode.attribute("name").as_string();
		auto gameObject = _game->Create(
			_scene,
			switchNode.attribute("actor").as_uint(),
			switchName,
			switchNode.attribute("source").as_string(),
			_x, _y + 64, _gridX, _gridY, _layer, _active
		);
	}
		break;

	case CBlock::ESpawn::FLOWER:
	{
		auto flowerNode = prefab.child("Prefab").child("Flower");
		std::string flowerName = _name + flowerNode.attribute("name").as_string();
		auto gameObject = _game->Create(
			_scene,
			flowerNode.attribute("actor").as_uint(),
			flowerName,
			flowerNode.attribute("source").as_string(),
			_x, _y, _gridX, _gridY, _layer, _active
		);
	}
	break;

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
