#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX								9991

#define SPR_MARIO_FIREBALL1			1001
#define SPR_MARIO_FIREBALL2			1002
#define SPR_MARIO_FIREBALL3			1003
#define SPR_MARIO_FIREBALL4			1004
#pragma endregion

#pragma region Animation ID
#define ANI_MARIO_FIREBALL			1001
#pragma endregion


class CMarioFireball : public CGameObject
{
public:
	CMarioFireball(
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
	float DRAG_FORCE = 0;
	float GRAVITY = 0;
	float INITIAL_FORCE = 0;

	/* Logic */
	float _decayInterval = 0;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);

	void OnCollisionWithPlatform(pCollision e);
	void OnCollisionWithPipe(pCollision e);
	void OnCollisionWithBrick(pCollision e);
	void OnCollisionWithBlock(pCollision e);

	void OnCollisionWithGoomba(pCollision e);
	void OnCollisionWithKoopa(pCollision e);
	void OnCollisionWithPiranha(pCollision e);
	void OnCollisionWithVenus(pCollision e);
};
typedef CMarioFireball* pMarioFireball;