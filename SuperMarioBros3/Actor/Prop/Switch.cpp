#include "Switch.h"
#include "../../SuperMarioBros3.h"
#include "../Item/Coin.h"
#include "../Prop/Brick.h"

void CSwitch::Load()
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
	TIMER = statsNode.attribute("TIMER").as_float();
}

void CSwitch::Start()
{
	_start = true;
	_activate = false;
	_end = false;
	_timer = TIMER;
	_animations[ANI_SWITCH]->Play(true);
}

void CSwitch::Update(float elapsedMs)
{
	if (!_start) Start();
	_animations[ANI_SWITCH]->Update(elapsedMs);

	if (!_end)
	{
		if (_activate)
		{
			_animations[ANI_SWITCH]->Stop();

			if (_timer > 0) _timer -= elapsedMs;
			else
			{
				Transform();
				_end = true;
			}
		}
	}
}

void CSwitch::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	if (!_activate)
		_animations[ANI_SWITCH]->Render(_x, _y);
	else
		_sprites[SPR_SWITCH_DOWN]->Render(_x, _y);
}

void CSwitch::Transform()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());
	auto coinNode = prefab.child("Prefab").child("Coin");
	auto brickNode = prefab.child("Prefab").child("Brick");

	for (auto& gameObject : _game->_updateQueue)
	{
		if (dynamic_cast<pCoin>(_game->Get(gameObject)))
		{
			auto coin = dynamic_cast<pCoin>(_game->Get(gameObject));
			if (!coin->_destroy)
			{
				coin->Destroy();
				std::string brickName = coin->_name + brickNode.attribute("name").as_string();
				_game->Create(
					_scene,
					brickNode.attribute("actor").as_uint(),
					brickName,
					brickNode.attribute("source").as_string(),
					coin->_x, coin->_y, coin->_gridX, coin->_gridY,
					coin->_layer, true
				);
			}
		}
		
		else if (dynamic_cast<pBrick>(_game->Get(gameObject)))
		{
			auto brick = dynamic_cast<pBrick>(_game->Get(gameObject));
			if (!brick->_destroy)
			{
				brick->Destroy();
				std::string coinName = brick->_name + coinNode.attribute("name").as_string();
				_game->Create(
					_scene,
					coinNode.attribute("actor").as_uint(),
					coinName,
					coinNode.attribute("source").as_string(),
					brick->_x, brick->_y, brick->_gridX, brick->_gridY,
					brick->_layer, true
				);
			}
		}
	}
}

int CSwitch::IsCollidable()
{
	if (!_activate) return 1;
	else return 0;
}

int CSwitch::IsBlocking()
{
	if (!_activate) return 1;
	else return 0;
}

void CSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CSwitch::OnNoCollision(float elapsedMs)
{
}

void CSwitch::OnCollisionWith(pCollision e)
{
}
