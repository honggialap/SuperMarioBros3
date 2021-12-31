#pragma once
#include "../../../../Engine/GameObject.h"

class CMario;
typedef CMario* pMario;

#pragma region Sprite ID
#define SPR_FIREBALL1			1001
#define SPR_FIREBALL2			1002
#define SPR_FIREBALL3			1003
#define SPR_FIREBALL4			1004
#pragma endregion

#pragma region Animation ID
#define ANI_FIREBALL			1001
#pragma endregion

class CMarioFireball : public CGameObject
{
public:
	CMarioFireball(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Master */
	pMario mario;

	/* Stats */

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Logic */
	enum class EAction
	{
		STANDBY,
		ENTRY,
		PROGRESS,
		RECOVER
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::STANDBY;
	EAction _nextAction = EAction::STANDBY;

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
	void Standby(float elapsedMs);
	void Entry(float elapsedMs);
	void Progress(float elapsedMs);
	void Recover(float elapsedMs);

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
	void OnCollisionWithItem(pCollision e);
};
typedef CMarioFireball* pMarioFireball;