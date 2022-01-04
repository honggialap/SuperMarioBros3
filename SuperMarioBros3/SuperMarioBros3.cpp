#include "SuperMarioBros3.h"

#include "Actor/Controller/GameMaster.h"
#include "Actor/Controller/World.h"

#include "Actor/Character/Mario.h"
#include "Actor/Character/MarioTail.h"
#include "Actor/Character/Goomba.h"
#include "Actor/Character/Koopa.h"
#include "Actor/Character/KoopaSensor.h"
#include "Actor/Character/Piranha.h"
#include "Actor/Character/PiranhaSensor.h"
#include "Actor/Character/Venus.h"
#include "Actor/Character/VenusFireball.h"
#include "Actor/Character/VenusSensor.h"

#include "Actor/Item/Coin.h"
#include "Actor/Item/ExtraLife.h"
#include "Actor/Item/Mushroom.h"
#include "Actor/Item/Leaf.h"

#include "Actor/Misc/Background.h"
#include "Actor/Misc/HUD.h"
#include "Actor/Misc/ScoreEffect.h"

#include "Actor/Prop/SolidPlatform.h"
#include "Actor/Prop/HollowedPlatform.h"
#include "Actor/Prop/Brick.h"
#include "Actor/Prop/Block.h"
#include "Actor/Prop/SpawnZone.h"
#include "Actor/Prop/DeadZone.h"
#include "Actor/Prop/Switch.h"
#include "Actor/Prop/Pipe.h"
#include "Actor/Prop/Goal.h"

pGameObject CSMB3::Create(pScene scene, unsigned int actorID, std::string name, std::string prefabSource, float posX, float posY, int gridX, int gridY, unsigned int layer, bool active)
{
	switch (actorID)
	{
		/* Controller */
	case ACT_GAMEMASTER:
	{
		auto gameObject = new CGameMaster(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_WORLD:
	{
		auto gameObject = new CWorld(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

		/* Character */
	case ACT_MARIO:
	{
		auto gameObject = new CMario(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_MARIO_TAIL:
	{
		auto gameObject = new CMarioTail(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_GOOMBA:
	{
		auto gameObject = new CGoomba(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_KOOPA:
	{
		auto gameObject = new CKoopa(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_KOOPA_SENSOR:
	{
		auto gameObject = new CKoopaSensor(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_PIRANHA:
	{
		auto gameObject = new CPiranha(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_PIRANHA_SENSOR:
	{
		auto gameObject = new CPiranhaSensor(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_VENUS:
	{
		auto gameObject = new CVenus(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_VENUS_FIREBALL:
	{
		auto gameObject = new CVenusFireball(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_VENUS_SENSOR:
	{
		auto gameObject = new CVenusSensor(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

		/* Item */
	case ACT_COIN:
	{
		auto gameObject = new CCoin(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_EXTRALIFE:
	{
		auto gameObject = new CExtraLife(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_MUSHROOM:
	{
		auto gameObject = new CMushroom(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_LEAF:
	{
		auto gameObject = new CLeaf(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

		/* Prop */
	case ACT_SOLID_PLATFORM:
	{
		auto gameObject = new CSolidPlatform(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_HOLLOWED_PLATFORM:
	{
		auto gameObject = new CHollowedPlatform(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_BRICK:
	{
		auto gameObject = new CBrick(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_BLOCK:
	{
		auto gameObject = new CBlock(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_SPAWN_ZONE:
	{
		auto gameObject = new CSpawnZone(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_DEAD_ZONE:
	{
		auto gameObject = new CDeadZone(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_SWITCH:
	{
		auto gameObject = new CSwitch(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_PIPE:
	{
		auto gameObject = new CPipe(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_GOAL:
	{
		auto gameObject = new CGoal(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

		/* Misc */
	case ACT_BACKGROUND:
	{
		auto gameObject = new CBackground(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_HUD:
	{
		auto gameObject = new CHUD(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	case ACT_SCORE_EFFECT:
	{
		auto gameObject = new CScoreEffect(
			this, scene, nextGameObjectId++,
			name, prefabSource,
			posX, posY, gridX, gridY, layer
		);
		Add(gameObject);
		if (active) gameObject->Active();
		gameObject->Load();
		return gameObject;
	}
	break;

	default:
		return nullptr;
		break;
	}
}
