#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX							9991

#define SPR_PIPE_GREEN_UP				1001
#define SPR_PIPE_GREEN_DOWN				1002
#define SPR_PIPE_BLACK_UP				1003
#define SPR_PIPE_BLACK_DOWN				1004
#pragma endregion


class CPipe : public CGameObject
{
public:
	CPipe(
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

	/* Logic */
	bool _green = false;
	bool _up = false;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CPipe* pPipe;