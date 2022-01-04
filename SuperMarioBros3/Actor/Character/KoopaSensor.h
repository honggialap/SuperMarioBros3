#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define LEFT_BBOX							9991
#define RIGHT_BBOX							9992
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CKoopaSensor : public CGameObject
{
public:
	CKoopaSensor(
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
	bool _left = false;
	bool _activate = false;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CKoopaSensor* pKoopaSensor;