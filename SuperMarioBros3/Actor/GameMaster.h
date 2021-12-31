	#pragma once
#include "Engine/GameObject.h"
#include "Intro/IntroController.h"
#include "WorldMap/WorldMapController.h"
#include "World/WorldController.h"

#pragma region Sprite ID
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CGameMaster : public CGameObject
{
public:
	CGameMaster(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Stats */
	unsigned int _scene = 1;
	unsigned int _marioLife = 0;
	unsigned int _marioPower = 0;
	unsigned int _score = 0;
	std::unordered_map<unsigned int, bool> _levelClear;

	/* Collision */
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CGameMaster* pGameMaster;