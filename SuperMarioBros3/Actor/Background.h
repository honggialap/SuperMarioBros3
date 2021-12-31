#pragma once
#include "../Engine/GameObject.h"

#pragma region Sprite ID
#pragma endregion

#pragma region Animation ID
#pragma endregion

#define BUFFER_CAM 128

class CBackground : public CGameObject
{
public:
	CBackground(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Tiled Map */
	unsigned int _mapWidth = 0;
	unsigned int _mapHeight = 0;
	float _cellWidth = 0;
	float _cellHeight = 0;
	std::vector<unsigned int> _cells;

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Logic */
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CBackground* pBackground;