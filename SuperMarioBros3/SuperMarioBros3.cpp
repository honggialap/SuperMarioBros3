#include "SuperMarioBros3.h"

#include "Actor/GameMaster.h"
#include "Actor/Background.h"
#include "Actor/HUD.h"

#include "Actor/Intro/IntroController.h"
#include "Actor/WorldMap/WorldMapController.h"

#include "Actor/World/WorldController.h"
#include "Actor/World/Score.h"
#include "Actor/World/Character/Goomba/Goomba.h"
#include "Actor/World/Character/Koopa/Koopa.h"
#include "Actor/World/Character/Koopa/KoopaSensor.h"
#include "Actor/World/Character/Mario/Mario.h"
#include "Actor/World/Character/Mario/MarioTail.h"
#include "Actor/World/Character/Mario/MarioFireball.h"
#include "Actor/World/Character/Plant/Piranha.h"
#include "Actor/World/Character/Plant/PiranhaSensor.h"
#include "Actor/World/Character/Plant/Venus.h"
#include "Actor/World/Character/Plant/VenusFireball.h"
#include "Actor/World/Character/Plant/VenusSensor.h"
#include "Actor/World/Item/Coin.h"
#include "Actor/World/Item/ExtraLife.h"
#include "Actor/World/Item/Flower.h"
#include "Actor/World/Item/Leaf.h"
#include "Actor/World/Item/Mushroom.h"
#include "Actor/World/Prop/DeadZone.h"
#include "Actor/World/Prop/Goal.h"
#include "Actor/World/Prop/Switch.h"
#include "Actor/World/Prop/Spawner.h"
#include "Actor/World/Prop/Block/Brick.h"
#include "Actor/World/Prop/Block/Block.h"
#include "Actor/World/Prop/Pipe/Pipe.h"
#include "Actor/World/Prop/Pipe/EntryPipe.h"
#include "Actor/World/Prop/Pipe/ExitPipe.h"
#include "Actor/World/Prop/Pipe/EmptyPipe.h"
#include "Actor/World/Prop/Platform/HollowedPlatform.h"
#include "Actor/World/Prop/Platform/SolidPlatform.h"

pGameObject CSMB3::Create(pScene scene, unsigned int actorID, std::string name, std::string prefabSource, float posX, float posY, int gridX, int gridY, unsigned int layer, bool active)
{
	switch (actorID)
	{

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

	case ACT_INTRO_CONTROLLER:
	{
		auto gameObject = new CIntroController(
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

	case ACT_WORLDMAP_CONTROLLER:
	{
		auto gameObject = new CWorldMapController(
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

	case ACT_WORLD_CONTROLLER:
	{
		auto gameObject = new CWorldController(
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

	case ACT_SCORE:
	{
		auto gameObject = new CScore(
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

	case ACT_MARIO_FIREBALL:
	{
		auto gameObject = new CMarioFireball(
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

	case ACT_FLOWER:
	{
		auto gameObject = new CFlower(
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

	case ACT_ENTRY_PIPE:
	{
		auto gameObject = new CEntryPipe(
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

	case ACT_EXIT_PIPE:
	{
		auto gameObject = new CExitPipe(
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

	case ACT_EMPTY_PIPE:
	{
		auto gameObject = new CEmptyPipe(
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

	case ACT_SPAWNER:
	{
		auto gameObject = new CSpawner(
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

	case ACT_DEADZONE:
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

	default:
		return nullptr;
		break;
	}
}
