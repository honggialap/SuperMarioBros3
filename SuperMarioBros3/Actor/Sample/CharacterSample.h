#pragma once
#include "Engine/GameObject.h"
#include "WeaponSample.h"



class CCharacterSample : public CGameObject
{
public:
	CCharacterSample(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	) : CGameObject(game, scene, id, name, prefabSource, posX, posY, gridX, gridY, layer) {};

	/* Binding Command */
	int UP = 0;
	int DOWN = 0;
	int LEFT = 0;
	int RIGHT = 0;
	int JUMP = 0;
	int ACTION = 0;

	/* Stats */
	float MOVE_SPEED = 0;
	float JUMP_FORCE = 0;
	float DRAG_FORCE = 0;
	float GRAVITY = 0;

	/* Lifecycle */
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	/* Logic */
	bool _isHold = false;
	bool _isLeft = false;
	bool _isGround = false;
	enum class EPower
	{
		SMALL,
		LARGE,
		FIRE,
		RACCOON
	};
	EPower _power = EPower::SMALL;
	//pWeaponSample _weapon;


	enum class EAction
	{
		IDLE,
		WALK,
		JUMP,
		HOVER,
		RUN_SLOW,
		RUN_FAST,
		RUN_FULL,
		FLY,
		CROUNCH,
		THROW_FIRE,
		SPIN_TAIL,
		KICK
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
	void Walk(float elapsedMs);
	void Jump(float elapsedMs);

	int IsCollidable() {};
	int IsBlocking() {};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollisionEvent e);
	void OnCollisionWithItem(pCollisionEvent e);
};
typedef CCharacterSample* pCharacterSample;