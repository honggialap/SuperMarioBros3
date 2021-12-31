#include "GameMaster.h"
#include "Engine/Game.h"
#include "Engine/Library/pugixml.hpp"

void CGameMaster::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	_scene = prefab.child("Prefab").child("Stats").attribute("startScene").as_uint();
	_marioLife = prefab.child("Prefab").child("Stats").attribute("marioLife").as_uint();
	_marioPower = prefab.child("Prefab").child("Stats").attribute("marioPower").as_uint();
	_score = 0;

	for (auto levelNode = prefab.child("Prefab").child("Level");
		levelNode;
		levelNode = levelNode.next_sibling("Level"))
	{
		_levelClear[
		levelNode.attribute("id").as_uint()
		] = false;
	}
}

void CGameMaster::Start()
{
	_start = true;
	_game->PlayScene(_scene);
}

void CGameMaster::Update(float elapsedMs)
{
	if (!_start) Start();
}

void CGameMaster::Render()
{
}

void CGameMaster::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
