#include "GameSample.h"
#include "Actor/Sample/ControllerSample.h"
#include "Actor/Sample/CharacterSample.h"
#include "Actor/Sample/EnemySample.h"
#include "Actor/Sample/ItemSample.h"
#include "Actor/Sample/PropSample.h"

pGameObject CGameSample::Create(
	pScene scene,
	unsigned int actorID, std::string name, std::string prefabSource,
	float posX, float posY, int gridX, int gridY,
	unsigned int layer, bool active
)
{
	switch (actorID)
	{

	case ACTOR_CONTROLLER_SAMPLE:
	{
		auto gameObject = new CControllerSample(
			this, scene, nextGameObjectId++, 
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;
	}
	break;

	case ACTOR_CHARACTER_SAMPLE:
	{
		auto gameObject = new CCharacterSample(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;	}
	break;

	case ACTOR_WEAPON_SAMPLE:
	{
		auto gameObject = new CWeaponSample(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;
	}
	break;

	case ACTOR_ENEMY_SAMPLE:
	{
		auto gameObject = new CEnemySample(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;
	}
	break;

	case ACTOR_ITEM_SAMPLE:
	{
		auto gameObject = new CItemSample(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;
	}
	break;

	case ACTOR_PROP_SAMPLE:
	{
		auto gameObject = new CPropSample(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		gameObject->Load();
		if (active) gameObject->Active();
		return gameObject;
	}
	break;

	default:
	{
		return nullptr;
	}

	}
}
