#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX						9991

#define SPR_SWITCH_UP1				1001
#define SPR_SWITCH_UP2				1002
#define SPR_SWITCH_DOWN				1003
#pragma endregion

#pragma region Animation ID
#define ANI_SWITCH					1001
#pragma endregion


class CSwitch : public CGameObject
{
public:
	CSwitch(
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
	float TIMER = 0;

	/* Logic */
	bool _activate = false;
	bool _end = false;
	float _timer = 0;

	void Transform();

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CSwitch* pSwitch;