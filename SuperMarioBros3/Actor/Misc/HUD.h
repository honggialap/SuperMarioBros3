#pragma once
#include "../../Engine/GameObject.h"
#include "../Character/Mario.h"

#pragma region Sprite ID
#define SPR_HUD_FRAME			1001
#define SPR_HUD_LEVEL1			2001
#define SPR_HUD_LEVEL2			2002
#define SPR_HUD_POWER1			3001
#define SPR_HUD_POWER2			3002
#pragma endregion

class CHUD : public CGameObject
{
public:
	CHUD(
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
	float POWER_LIMIT = 0;
	float BLINK_INTERVAL = 0;

	float FRAME_X = 0;
	float FRAME_Y = 0;

	float LEVEL1_X = 0;
	float LEVEL1_Y = 0;

	float LEVEL2_X = 0;
	float LEVEL2_Y = 0;

	float LEVEL3_X = 0;
	float LEVEL3_Y = 0;

	float LEVEL4_X = 0;
	float LEVEL4_Y = 0;

	float LEVEL5_X = 0;
	float LEVEL5_Y = 0;

	float LEVEL6_X = 0;
	float LEVEL6_Y = 0;

	float POWER_X = 0;
	float POWER_Y = 0;

	/* Logic */
	pMario _mario = nullptr;
	unsigned int _level = 0;
	bool _blink = false;
	float _blinkInterval = 0;

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CHUD* pHUD;