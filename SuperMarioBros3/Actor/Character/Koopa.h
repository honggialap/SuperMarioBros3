#pragma once
#include "../../Engine/GameObject.h"
#include "KoopaSensor.h"

#pragma region Sprite ID
#define BBOX							9991

#define SPR_KOOPA_WALK1_LEFT			1001
#define SPR_KOOPA_WALK2_LEFT			1002
#define SPR_KOOPA_WALK1_RIGHT			1101
#define SPR_KOOPA_WALK2_RIGHT			1102
#define SPR_KOOPA_RETRACT1_DOWN			1201
#define SPR_KOOPA_RETRACT2_DOWN			1202
#define SPR_KOOPA_SPIN1_DOWN			1203
#define SPR_KOOPA_SPIN2_DOWN			1204
#define SPR_KOOPA_SPIN3_DOWN			1205
#define SPR_KOOPA_RETRACT1_UP			1301
#define SPR_KOOPA_RETRACT2_UP			1302
#define SPR_KOOPA_SPIN1_UP				1303
#define SPR_KOOPA_SPIN2_UP				1304
#define SPR_KOOPA_SPIN3_UP				1305

#define SPR_RED_KOOPA_WALK1_LEFT		2001
#define SPR_RED_KOOPA_WALK2_LEFT		2002
#define SPR_RED_KOOPA_WALK1_RIGHT		2101
#define SPR_RED_KOOPA_WALK2_RIGHT		2102
#define SPR_RED_KOOPA_RETRACT1_DOWN		2201
#define SPR_RED_KOOPA_RETRACT2_DOWN		2202
#define SPR_RED_KOOPA_SPIN1_DOWN		2203
#define SPR_RED_KOOPA_SPIN2_DOWN		2204
#define SPR_RED_KOOPA_SPIN3_DOWN		2205
#define SPR_RED_KOOPA_RETRACT1_UP		2301
#define SPR_RED_KOOPA_RETRACT2_UP		2302
#define SPR_RED_KOOPA_SPIN1_UP			2303
#define SPR_RED_KOOPA_SPIN2_UP			2304
#define SPR_RED_KOOPA_SPIN3_UP			2305

#define SPR_KOOPA_WING1_LEFT			3001
#define SPR_KOOPA_WING2_LEFT			3002
#define SPR_KOOPA_WING1_RIGHT			3101
#define SPR_KOOPA_WING2_RIGHT			3102
#pragma endregion

#pragma region Animation ID
#define ANI_KOOPA_WALK_LEFT				1001
#define ANI_KOOPA_WALK_RIGHT			1101
#define ANI_KOOPA_SPIN_DOWN				1201
#define ANI_KOOPA_BLINK_DOWN			1202
#define ANI_KOOPA_SPIN_UP				1301
#define ANI_KOOPA_BLINK_UP				1302

#define ANI_RED_KOOPA_WALK_LEFT			2001
#define ANI_RED_KOOPA_WALK_RIGHT		2101
#define ANI_RED_KOOPA_SPIN_DOWN			2201
#define ANI_RED_KOOPA_BLINK_DOWN		2202
#define ANI_RED_KOOPA_SPIN_UP			2301
#define ANI_RED_KOOPA_BLINK_UP			2302

#define ANI_KOOPA_WING_LEFT				3001
#define ANI_KOOPA_WING_RIGHT			3101
#pragma endregion


class CKoopa : public CGameObject
{
public:
	CKoopa(
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
	pKoopaSensor _sensor = nullptr;


	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;


	/* Stats */
	float GRAVITY = 0;
	float WALK_SPEED = 0;
	float SPIN_SPEED = 0;
	float JUMP_FORCE = 0;
	float RESTORE_LIMIT = 0;
	float DECAY_INTERVAL = 0;


	/* Logic */
	enum class EType
	{
		KOOPA,
		RED_KOOPA
	};
	EType _type = EType::KOOPA;
	bool _wing = false;
	bool _left = false;
	bool _up = false;
	bool _ground = false;
	float _restoreLimit = 0;
	float _decayInterval = 0;


	/* State */
	enum class EAction
	{
		MOVE,
		RETRACT,
		SPIN,
		THROWN,
	};
	enum class EActionStage
	{
		START,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::START;
	EAction _action = EAction::MOVE;
	EAction _nextAction = EAction::MOVE;

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
	void Move(float elapsedMs);
	void Retract(float elapsedMs);
	void Spin(float elapsedMs);
	void Thrown(float elapsedMs);

	void UpdateSensor();
	void HitTop();
	void HitSide();
	void HitByTail(bool left);

	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);

	void OnCollisionWithGoomba(pCollision e);
	void OnCollisionWithKoopa(pCollision e);
	void OnCollisionWithPiranha(pCollision e);
	void OnCollisionWithVenus(pCollision e);

	void OnCollisionWithPlatform(pCollision e);
	void OnCollisionWithPipe(pCollision e);
	void OnCollisionWithBrick(pCollision e);
	void OnCollisionWithBlock(pCollision e);
	void OnCollisionWithDeadZone(pCollision e);

};
typedef CKoopa* pKoopa;