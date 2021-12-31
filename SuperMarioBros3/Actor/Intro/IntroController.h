#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CIntroController : public CGameObject
{
public:
	CIntroController(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Collision */
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CIntroController* pIntroController;