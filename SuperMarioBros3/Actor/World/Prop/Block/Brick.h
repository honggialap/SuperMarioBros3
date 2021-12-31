#pragma once
#include "../../../../Engine/GameObject.h"

#pragma region Sprite ID
#define BBOX						9991

#define SPR_BRICK1					2001
#define SPR_BRICK2					2002
#define SPR_BRICK3					2003
#define SPR_BRICK4					2004
#define SPR_BRICK_FRAG1				3001
#define SPR_BRICK_FRAG2				3002
#pragma endregion

#pragma region Animation ID
#define ANI_BRICK					2001
#pragma endregion


class CBrick : public CGameObject
{
public:
	CBrick(
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
	bool _return = false;
	float _originY = 0;


	enum class EAction
	{
		IDLE,
		BOUNCE,
		BROKE,
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
	void Bounce(float elapsedMs);
	void Broke(float elapsedMs);

	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CBrick* pBrick;