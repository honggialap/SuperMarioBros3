#pragma once
#include "../../Engine/GameObject.h"
#include "PiranhaSensor.h"

#pragma region Sprite ID
#define BBOX					9991

#define SPR_PIRANHA1			1201
#define SPR_PIRANHA2			1202
#define SPR_RED_PIRANHA1		2201
#define SPR_RED_PIRANHA2		2202
#pragma endregion

#pragma region Animation ID
#define ANI_PIRANHA				1201
#define ANI_RED_PIRANHA			2201
#pragma endregion


class CPiranha : public CGameObject
{
public:
	CPiranha(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};


	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();


	/* Child */
	pPiranhaSensor _sensor = nullptr;


	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;


	/* Stats */
	float HIDE_INTERVAL = 0;
	float STAND_INTERVAL = 0;
	float GROW_LIMIT = 0;
	float BURROW_LIMIT = 0;
	float SPEED = 0;

	/* Logic */
	float _hideInterval = 0;
	float _standInterval = 0;
	float _growLimit = 0;
	float _burrowLimit = 0;


	/* State */
	enum class EAction
	{
		HIDE,
		GROW,
		STAND,
		BURROW,
		DIE
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::HIDE;
	EAction _nextAction = EAction::HIDE;

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
	void Hide(float elapsedMs);
	void Grow(float elapsedMs);
	void Stand(float elapsedMs);
	void Burrow(float elapsedMs);
	void Die(float elapsedMs);

	void UpdateSensor();

	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CPiranha* pPiranha;