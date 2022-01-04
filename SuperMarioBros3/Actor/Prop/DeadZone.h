#pragma once
#include "../../Engine/GameObject.h"

#pragma region Sprite ID
#pragma endregion

#pragma region Animation ID
#pragma endregion


class CDeadZone : public CGameObject
{
public:
	CDeadZone(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Binding Command */
	/* Stats */
	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Logic */
	enum class EAction
	{
		DEFAULT
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::DEFAULT;
	EAction _nextAction = EAction::DEFAULT;

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

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CDeadZone* pDeadZone;