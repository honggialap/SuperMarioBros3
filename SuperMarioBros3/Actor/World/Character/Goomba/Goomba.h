#pragma once
#include "Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX							9991

#define SPR_GOOMBA_WALK1				1001
#define SPR_GOOMBA_WALK2				1002
#define SPR_GOOMBA_DIE					1003
#define SPR_RED_GOOMBA_WALK1			2001
#define SPR_RED_GOOMBA_WALK2			2002
#define SPR_RED_GOOMBA_DIE				2003
#define SPR_GOOMBA_WING1				3001
#define SPR_GOOMBA_WING2				3002
#pragma endregion

#pragma region Animation ID
#define ANI_GOOMBA_WALK					1001
#define ANI_RED_GOOMBA_WALK				2001
#define ANI_GOOMBA_WING_FLAP			3001
#pragma endregion


class CGoomba : public CGameObject
{
public:
	CGoomba(
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
	float GRAVITY = 0;
	float WALK_SPEED = 0;
	float JUMP_FORCE = 0;
	float DECAY_INTERVAL = 0;
	float JUMP_INTERVAL = 0;


	/* Logic */
	enum class EType
	{
		GOOMBA,
		RED_GOOMBA
	};
	EType _type = EType::GOOMBA;
	bool _wing = false;
	bool _left = false;
	bool _ground = false;
	unsigned int _targetId = 0;
	float _currentDecayInterval = 0;
	float _currentJumpInterval = 0;


	/* State */
	enum class EAction
	{
		WALK,
		JUMP,
		DIE,
		THROWN
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::WALK;
	EAction _nextAction = EAction::WALK;

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
	void Walk(float elapsedMs);
	void Jump(float elapsedMs);
	void Die(float elapsedMs);
	void Thrown(float elapsedMs);


	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CGoomba* pGoomba;