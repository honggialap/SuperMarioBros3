#pragma once
#include "Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX							9991
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CSolidPlatform : public CGameObject
{
public:
	CSolidPlatform(
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


	/* Collision */
	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void OnNoCollision(float elapsedMs);
	virtual void OnCollisionWith(pCollision e);
};
typedef CSolidPlatform* pSolidPlatform;