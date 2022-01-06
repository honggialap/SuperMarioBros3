#include "SpawnZone.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Mario.h"

void CSpawnZone::Load()
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
	SPAWN_COUNTDOWN = statsNode.attribute("SPAWN_COUNTDOWN").as_float();
}

void CSpawnZone::Start()
{
	_start = true;
	_activate = false;
	_spawnCountdown = SPAWN_COUNTDOWN;
}

void CSpawnZone::Update(float elapsedMs)
{
	if (!_start) Start();

	_activate = false;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);

	if (_spawnCountdown > 0) _spawnCountdown -= elapsedMs;
	else
	{
		if (_activate)
		{
			Spawn();
			_spawnCountdown = SPAWN_COUNTDOWN;
		}
	}
}

void CSpawnZone::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
}

void CSpawnZone::Spawn()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	for (auto gameObjectNode = prefab.child("Prefab").child("GameObject");
		gameObjectNode;
		gameObjectNode = gameObjectNode.next_sibling("GameObject"))
	{
		if (_game->Get(gameObjectNode.attribute("name").as_string()) == nullptr)
		{
			_game->Create(
				_scene,
				gameObjectNode.attribute("actor").as_uint(),
				gameObjectNode.attribute("name").as_string(),
				gameObjectNode.attribute("source").as_string(),
				gameObjectNode.attribute("x").as_float(),
				gameObjectNode.attribute("y").as_float(),
				_gridX, _gridY,_layer, _active
			);
		}
	}
}

int CSpawnZone::IsCollidable()
{
	return 1;
}

int CSpawnZone::IsBlocking()
{
	return 0;
}

void CSpawnZone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CSpawnZone::OnNoCollision(float elapsedMs)
{
}

void CSpawnZone::OnCollisionWith(pCollision e)
{
	if (dynamic_cast<pMario>(e->_target))
		_activate = true;
}
