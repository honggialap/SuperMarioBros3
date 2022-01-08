#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define SPR_BRICK_FRAG_LEFT		1001
#define SPR_BRICK_FRAG_RIGHT	1002
#pragma endregion

class CBrickFrag : public CGameObject
{
public:
	CBrickFrag(
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
	float DECAY_INTERVAL = 0;
	float GRAVITY = 0;
	float DRAG_FORCE = 0;
	float INITIAL_FORCE = 0;

	/* Logic */
	float _decayInterval = 0;
	bool _left = false;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CBrickFrag* pBrickFrag;