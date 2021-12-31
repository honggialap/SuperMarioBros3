#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CWorldMapController : public CGameObject
{
public:
	CWorldMapController(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* State */
	enum class EAction
	{
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action;
	EAction _nextAction;

	void SetAction(EAction action)
	{
		_action = action;
		_actionStage = EActionStage::START;
	};
	void SetNextAction(EAction action)
	{
		_nextAction = action;
		_actionStage = EActionStage::EXIT;
	};
	void NextAction()
	{
		_action = _nextAction;
		_actionStage = EActionStage::START;
	};

	void HandleAction(float elapsedMs);

	/* Collision */
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CWorldMapController* pWorldMapController;