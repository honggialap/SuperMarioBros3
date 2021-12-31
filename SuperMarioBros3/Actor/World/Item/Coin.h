#pragma once
#include "../../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX				9991

#define SPR_COIN1			1001
#define SPR_COIN2			1002
#define SPR_COIN3			1003
#define SPR_COIN4			1004
#define SPR_COIN5			1005
#define SPR_COIN6			1006
#pragma endregion

#pragma region Animation ID
#define ANI_COIN			1001
#pragma endregion


class CCoin : public CGameObject
{
public:
	CCoin(
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
	float DECAY_INTERVAL = 0;
	float SPEED = 0;

	/* Logic */
	float _currentDecayInterval = 0;

	/* State */
	enum class EAction
	{
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
	EAction _action = EAction::IDLE;
	EAction _nextAction = EAction::IDLE;

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
	void Idle(float elapsedMs);
	void Consumed(float elapsedMs);


	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CCoin* pCoin;