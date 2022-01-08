#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX					9991
#define SPR_FLOWER				1001
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CFlower : public CGameObject
{
public:
	CFlower(
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
	float MOVE_SPEED = 0;
	float SPAWN_LIMIT = 0;

	/* Logic */
	float _left = false;
	float _spawnLimit = 0;

	/* State */
	enum class EAction
	{
		SPAWN,
		IDLE,
		CONSUMED
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::SPAWN;
	EAction _nextAction = EAction::SPAWN;;

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
	void Spawn(float elapsedMs);
	void Idle(float elapsedMs);
	void Consumed(float elapsedMs);

	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CFlower* pFlower;