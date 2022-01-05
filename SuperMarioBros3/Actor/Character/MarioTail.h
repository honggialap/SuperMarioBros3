#pragma once
#include "../../Engine/GameObject.h"


#pragma region Sprite ID
/* BBOX */
#define BBOX_LEFT						9991
#define BBOX_RIGHT						9992
#pragma endregion


class CMarioTail : public CGameObject
{
public:
	CMarioTail(
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
	float LEFT_BODY_WIDTH = 0;
	float LEFT_BODY_HEIGHT = 0;
	float LEFT_BODY_OFFSETX = 0;
	float LEFT_BODY_OFFSETY = 0;
	float RIGHT_BODY_WIDTH = 0;
	float RIGHT_BODY_HEIGHT = 0;
	float RIGHT_BODY_OFFSETX = 0;
	float RIGHT_BODY_OFFSETY = 0;


	/* Logic */
	bool _left;
	bool _activate;


	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);

	void OnCollisionWithGoomba(pCollision e);
	void OnCollisionWithKoopa(pCollision e);

};
typedef CMarioTail* pMarioTail;