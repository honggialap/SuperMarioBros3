#include "GameMaster.h"
#include "../../SuperMarioBros3.h"

void CGameMaster::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_marioLife = statsNode.attribute("marioLife").as_uint();
	_worldIndex = statsNode.attribute("worldIndex").as_uint();
}

void CGameMaster::Start()
{
	_start = true;
	_skipFrame = false;
	_game->PlayScene(_worldIndex);
}

void CGameMaster::Update(float elapsedMs)
{
	if (!_start) Start();

	if (_skipFrame)
	{
		_game->PlayScene(_worldIndex);
		_skipFrame = false;
	}
}

void CGameMaster::Render()
{
}

void CGameMaster::Restart()
{
	_skipFrame = true;
	_game->StopScene(_worldIndex);
}

void CGameMaster::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
