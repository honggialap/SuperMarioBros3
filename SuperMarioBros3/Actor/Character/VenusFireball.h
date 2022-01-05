#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define SPR_VENUS_FIREBALL1			1001
#define SPR_VENUS_FIREBALL2			1002
#define SPR_VENUS_FIREBALL3			1003
#define SPR_VENUS_FIREBALL4			1004
#pragma endregion

#pragma region Animation ID
#define ANI_VENUS_FIREBALL			1001
#pragma endregion


class CVenusFireball : public CGameObject
{
public:
	CVenusFireball(
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
	float DECAY_INTERVAL = 0;

	/* Logic */
	float _decayInterval = 0;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CVenusFireball* pVenusFireball;