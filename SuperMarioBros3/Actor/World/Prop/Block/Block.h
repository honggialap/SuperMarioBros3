#pragma once
#include "Engine/GameObject.h"
#include "Actor/World/Character/Mario/Mario.h"

#pragma region Sprite ID
#define BBOX							9991

#define SPR_BLOCK_QUESTION1				1001
#define SPR_BLOCK_QUESTION2				1002
#define SPR_BLOCK_QUESTION3				1003
#define SPR_BLOCK_QUESTION4				1004
#define SPR_BRICK1						2001
#define SPR_BRICK2						2002
#define SPR_BRICK3						2003
#define SPR_BRICK4						2004
#define SPR_BLOCK_EMPTY					3001
#pragma endregion

#pragma region Animation ID
#define ANI_BLOCK_QUESTION				1001
#define ANI_BRICK						2001
#pragma endregion


class CBlock : public CGameObject
{
public:
	CBlock(
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
	float BOUNCE_LIMIT = 0;


	/* Logic */
	bool _topBounce;
	bool _leftBounce;
	bool _return = false;
	float _origin = 0;
	pMario _target = nullptr;

	enum class EType
	{
		QUESTION,
		INVISIBLE,
		HIDDEN
	};
	EType _type = EType::QUESTION;
	enum class ESpawn
	{
		COIN,
		POWER_UP,
		EXTRA_LIFE,
		SWITCH
	};
	ESpawn _spawn = ESpawn::COIN;


	/* State */
	enum class EAction
	{
		IDLE,
		SPAWN,
		EMTPY,
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
	void Idle(float elapsedMs);
	void Spawn(float elapsedMs);
	void Empty(float elapsedMs);
	void SpawnItem();

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CBlock* pBlock;