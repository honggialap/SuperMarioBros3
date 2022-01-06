#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX							9991

#pragma endregion

#pragma region Animation ID
#pragma endregion


class CSpawnZone : public CGameObject
{
public:
	CSpawnZone(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Stats */
	float SPAWN_COUNTDOWN = 0;

	/* Logic */
	bool _activate = false;
	float _spawnCountdown = 0;

	void Spawn();

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CSpawnZone* pSpawnZone;