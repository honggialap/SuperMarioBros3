#pragma once
#include "../../Engine/GameObject.h"
#include "../Character/Koopa.h"
#include "../Prop/Pipe.h"
#include "../Prop/TransportPipe.h"
#include "MarioTail.h"


#pragma region Sprite ID
/* BBOX */
#define BBOX_SMALL						9991
#define BBOX_LARGE						9992

/* SMALL */
#define SPR_MARIO_S_IDLE_LEFT			1001
#define SPR_MARIO_S_WALK_LEFT			1002
#define SPR_MARIO_S_JUMP_LEFT			1003
#define SPR_MARIO_S_RUN1_LEFT			1004
#define SPR_MARIO_S_RUN2_LEFT			1005
#define SPR_MARIO_S_FLY_LEFT			1006
#define SPR_MARIO_S_DRIFT_LEFT			1007
#define SPR_MARIO_S_HOLD_IDLE_LEFT		1008
#define SPR_MARIO_S_HOLD_WALK_LEFT		1009
#define SPR_MARIO_S_KICK_LEFT			1010

#define SPR_MARIO_S_IDLE_RIGHT			1101
#define SPR_MARIO_S_WALK_RIGHT			1102
#define SPR_MARIO_S_JUMP_RIGHT			1103
#define SPR_MARIO_S_RUN1_RIGHT			1104
#define SPR_MARIO_S_RUN2_RIGHT			1105
#define SPR_MARIO_S_FLY_RIGHT			1106
#define SPR_MARIO_S_DRIFT_RIGHT			1107
#define SPR_MARIO_S_HOLD_IDLE_RIGHT		1108
#define SPR_MARIO_S_HOLD_WALK_RIGHT		1109
#define SPR_MARIO_S_KICK_RIGHT			1110

#define SPR_MARIO_S_DIE					1201
#define SPR_MARIO_S_FRONT				1202

/* LARGE */
#define SPR_MARIO_L_IDLE_LEFT			2001
#define SPR_MARIO_L_WALK1_LEFT			2002
#define SPR_MARIO_L_WALK2_LEFT			2003
#define SPR_MARIO_L_CROUCH_LEFT			2004
#define SPR_MARIO_L_JUMP_LEFT			2005
#define SPR_MARIO_L_RUN1_LEFT			2006
#define SPR_MARIO_L_RUN2_LEFT			2007
#define SPR_MARIO_L_RUN3_LEFT			2008
#define SPR_MARIO_L_FLY_LEFT			2009
#define SPR_MARIO_L_DRIFT_LEFT			2010
#define SPR_MARIO_L_HOLD_IDLE_LEFT		2011
#define SPR_MARIO_L_HOLD_WALK1_LEFT		2012
#define SPR_MARIO_L_HOLD_WALK2_LEFT		2013
#define SPR_MARIO_L_KICK_LEFT			2014
#define SPR_MARIO_L_GROW_LEFT			2015

#define SPR_MARIO_L_IDLE_RIGHT			2101
#define SPR_MARIO_L_WALK1_RIGHT			2102
#define SPR_MARIO_L_WALK2_RIGHT			2103
#define SPR_MARIO_L_CROUCH_RIGHT		2104
#define SPR_MARIO_L_JUMP_RIGHT			2105
#define SPR_MARIO_L_RUN1_RIGHT			2106
#define SPR_MARIO_L_RUN2_RIGHT			2107
#define SPR_MARIO_L_RUN3_RIGHT			2108
#define SPR_MARIO_L_FLY_RIGHT			2109
#define SPR_MARIO_L_DRIFT_RIGHT			2110
#define SPR_MARIO_L_HOLD_IDLE_RIGHT		2111
#define SPR_MARIO_L_HOLD_WALK1_RIGHT	2112
#define SPR_MARIO_L_HOLD_WALK2_RIGHT	2113
#define SPR_MARIO_L_KICK_RIGHT			2114
#define SPR_MARIO_L_GROW_RIGHT			2115

#define SPR_MARIO_L_FRONT				2201
#define SPR_MARIO_L_BACK				2202

/* RACCOON */
#define SPR_MARIO_R_IDLE_LEFT			3001
#define SPR_MARIO_R_WALK1_LEFT			3002
#define SPR_MARIO_R_WALK2_LEFT			3003
#define SPR_MARIO_R_CROUCH_LEFT			3004
#define SPR_MARIO_R_JUMP_LEFT			3005
#define SPR_MARIO_R_HOVER1_LEFT			3006
#define SPR_MARIO_R_HOVER2_LEFT			3007
#define SPR_MARIO_R_RUN1_LEFT			3008
#define SPR_MARIO_R_RUN2_LEFT			3009
#define SPR_MARIO_R_RUN3_LEFT			3010
#define SPR_MARIO_R_FLY1_LEFT			3011
#define SPR_MARIO_R_FLY2_LEFT			3012
#define SPR_MARIO_R_FLY3_LEFT			3013
#define SPR_MARIO_R_DRIFT_LEFT			3014
#define SPR_MARIO_R_HOLD_IDLE_LEFT		3015
#define SPR_MARIO_R_HOLD_WALK1_LEFT		3016
#define SPR_MARIO_R_HOLD_WALK2_LEFT		3017
#define SPR_MARIO_R_KICK_LEFT			3018
										
#define SPR_MARIO_R_IDLE_RIGHT			3101
#define SPR_MARIO_R_WALK1_RIGHT			3102
#define SPR_MARIO_R_WALK2_RIGHT			3103
#define SPR_MARIO_R_CROUCH_RIGHT		3104
#define SPR_MARIO_R_JUMP_RIGHT			3105
#define SPR_MARIO_R_HOVER1_RIGHT		3106
#define SPR_MARIO_R_HOVER2_RIGHT		3107
#define SPR_MARIO_R_RUN1_RIGHT			3108
#define SPR_MARIO_R_RUN2_RIGHT			3109
#define SPR_MARIO_R_RUN3_RIGHT			3110
#define SPR_MARIO_R_FLY1_RIGHT			3111
#define SPR_MARIO_R_FLY2_RIGHT			3112
#define SPR_MARIO_R_FLY3_RIGHT			3113
#define SPR_MARIO_R_DRIFT_RIGHT			3114
#define SPR_MARIO_R_HOLD_IDLE_RIGHT		3115
#define SPR_MARIO_R_HOLD_WALK1_RIGHT	3116
#define SPR_MARIO_R_HOLD_WALK2_RIGHT	3117
#define SPR_MARIO_R_KICK_RIGHT			3118
										
#define SPR_MARIO_R_FRONT				3201
#define SPR_MARIO_R_LEFT				3202
#define SPR_MARIO_R_BACK				3203
#define SPR_MARIO_R_RIGHT				3204

/* FIRE */
#define SPR_MARIO_F_IDLE_LEFT			4001
#define SPR_MARIO_F_WALK1_LEFT			4002
#define SPR_MARIO_F_WALK2_LEFT			4003
#define SPR_MARIO_F_CROUCH_LEFT			4004
#define SPR_MARIO_F_JUMP_LEFT			4005
#define SPR_MARIO_F_RUN1_LEFT			4006
#define SPR_MARIO_F_RUN2_LEFT			4007
#define SPR_MARIO_F_RUN3_LEFT			4008
#define SPR_MARIO_F_FLY_LEFT			4009
#define SPR_MARIO_F_DRIFT_LEFT			4010
#define SPR_MARIO_F_HOLD_IDLE_LEFT		4011
#define SPR_MARIO_F_HOLD_WALK1_LEFT		4012
#define SPR_MARIO_F_HOLD_WALK2_LEFT		4013
#define SPR_MARIO_F_KICK_LEFT			4014
#define SPR_MARIO_F_ATTACK1_LEFT		4015
#define SPR_MARIO_F_ATTACK2_LEFT		4016

#define SPR_MARIO_F_IDLE_RIGHT			4101
#define SPR_MARIO_F_WALK1_RIGHT			4102
#define SPR_MARIO_F_WALK2_RIGHT			4103
#define SPR_MARIO_F_CROUCH_RIGHT		4104
#define SPR_MARIO_F_JUMP_RIGHT			4105
#define SPR_MARIO_F_RUN1_RIGHT			4106
#define SPR_MARIO_F_RUN2_RIGHT			4107
#define SPR_MARIO_F_RUN3_RIGHT			4108
#define SPR_MARIO_F_FLY_RIGHT			4109
#define SPR_MARIO_F_DRIFT_RIGHT			4110
#define SPR_MARIO_F_HOLD_IDLE_RIGHT		4111
#define SPR_MARIO_F_HOLD_WALK1_RIGHT	4112
#define SPR_MARIO_F_HOLD_WALK2_RIGHT	4113
#define SPR_MARIO_F_KICK_RIGHT			4114
#define SPR_MARIO_F_ATTACK1_RIGHT		4115
#define SPR_MARIO_F_ATTACK2_RIGHT		4116

#define SPR_MARIO_F_FRONT				4201
#define SPR_MARIO_F_BACK				4202

										
#define SPR_MARIO_TRANSFORM1			5001
#define SPR_MARIO_TRANSFORM2			5002
#define SPR_MARIO_TRANSFORM3			5003
#define SPR_MARIO_TRANSFORM4			5004
#define SPR_MARIO_TRANSFORM5			5005
#define SPR_MARIO_TRANSFORM6			5006
#pragma endregion

#pragma region Animation ID
/* SMALL */
#define ANI_MARIO_S_WALK_LEFT			1001
#define ANI_MARIO_S_RUN_LEFT			1002
#define ANI_MARIO_S_RUN_FULL_LEFT		1003
#define ANI_MARIO_S_HOLD_WALK_LEFT		1004
#define ANI_MARIO_S_HOLD_RUN_LEFT		1005
#define ANI_MARIO_S_HOLD_RUN_FULL_LEFT	1006

#define ANI_MARIO_S_WALK_RIGHT			1101
#define ANI_MARIO_S_RUN_RIGHT			1102
#define ANI_MARIO_S_RUN_FULL_RIGHT		1103
#define ANI_MARIO_S_HOLD_WALK_RIGHT		1104
#define ANI_MARIO_S_HOLD_RUN_RIGHT		1105
#define ANI_MARIO_S_HOLD_RUN_FULL_RIGHT	1106

/* Large */
#define ANI_MARIO_L_WALK_LEFT			2001
#define ANI_MARIO_L_RUN_LEFT			2002
#define ANI_MARIO_L_RUN_FULL_LEFT		2003
#define ANI_MARIO_L_HOLD_WALK_LEFT		2004
#define ANI_MARIO_L_HOLD_RUN_LEFT		2005
#define ANI_MARIO_L_HOLD_RUN_FULL_LEFT	2006
#define ANI_MARIO_L_GROW_LEFT			2007
#define ANI_MARIO_L_DEGROW_LEFT			2008

#define ANI_MARIO_L_WALK_RIGHT			2101
#define ANI_MARIO_L_RUN_RIGHT			2102
#define ANI_MARIO_L_RUN_FULL_RIGHT		2103
#define ANI_MARIO_L_HOLD_WALK_RIGHT		2104
#define ANI_MARIO_L_HOLD_RUN_RIGHT		2105
#define ANI_MARIO_L_HOLD_RUN_FULL_RIGHT	2106
#define ANI_MARIO_L_GROW_RIGHT			2107
#define ANI_MARIO_L_DEGROW_RIGHT		2108

/* Raccoon */
#define ANI_MARIO_R_WALK_LEFT			3001
#define ANI_MARIO_R_HOVER_LEFT			3002
#define ANI_MARIO_R_RUN_LEFT			3003
#define ANI_MARIO_R_RUN_FULL_LEFT		3004
#define ANI_MARIO_R_FLY_LEFT			3005
#define ANI_MARIO_R_HOLD_WALK_LEFT		3006
#define ANI_MARIO_R_HOLD_RUN_LEFT		3007
#define ANI_MARIO_R_HOLD_RUN_FULL_LEFT	3008
#define ANI_MARIO_R_ATTACK_LEFT			3009
										
#define ANI_MARIO_R_WALK_RIGHT			3101
#define ANI_MARIO_R_HOVER_RIGHT			3102
#define ANI_MARIO_R_RUN_RIGHT			3103
#define ANI_MARIO_R_RUN_FULL_RIGHT		3104
#define ANI_MARIO_R_FLY_RIGHT			3105
#define ANI_MARIO_R_HOLD_WALK_RIGHT		3106
#define ANI_MARIO_R_HOLD_RUN_RIGHT		3107
#define ANI_MARIO_R_HOLD_RUN_FULL_RIGHT	3108
#define ANI_MARIO_R_ATTACK_RIGHT		3109
										
#define ANI_MARIO_R_TRANSFORM			5001

/* Large */
#define ANI_MARIO_F_WALK_LEFT			4001
#define ANI_MARIO_F_RUN_LEFT			4002
#define ANI_MARIO_F_RUN_FULL_LEFT		4003
#define ANI_MARIO_F_HOLD_WALK_LEFT		4004
#define ANI_MARIO_F_HOLD_RUN_LEFT		4005
#define ANI_MARIO_F_HOLD_RUN_FULL_LEFT	4006
#define ANI_MARIO_F_ATTACK_LEFT			4007
				  						
#define ANI_MARIO_F_WALK_RIGHT			4101
#define ANI_MARIO_F_RUN_RIGHT			4102
#define ANI_MARIO_F_RUN_FULL_RIGHT		4103
#define ANI_MARIO_F_HOLD_WALK_RIGHT		4104
#define ANI_MARIO_F_HOLD_RUN_RIGHT		4105
#define ANI_MARIO_F_HOLD_RUN_FULL_RIGHT	4106
#define ANI_MARIO_F_ATTACK_RIGHT		4107
#pragma endregion


class CMario : public CGameObject
{
public:
	CMario(
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
	pMarioTail _tail = nullptr;
	pKoopa _shell = nullptr;
	pTransportPipe _pipe = nullptr;

	/* Body */
	bool _renderBody = false;
	float SMALL_BODY_WIDTH = 0;
	float SMALL_BODY_HEIGHT = 0;
	float SMALL_BODY_OFFSETX = 0;
	float SMALL_BODY_OFFSETY = 0;
	float LARGE_BODY_WIDTH = 0;
	float LARGE_BODY_HEIGHT = 0;
	float LARGE_BODY_OFFSETX = 0;
	float LARGE_BODY_OFFSETY = 0;

	/* Binding Command */
	int UP = 0;
	int DOWN = 0;
	int LEFT = 0;
	int RIGHT = 0;
	int JUMP = 0;
	int ACTION = 0;
	int HOTKEY1 = 0;
	int HOTKEY2 = 0;
	int HOTKEY3 = 0;
	int HOTKEY4 = 0;
	int HOTKEY5 = 0;

	/* Stats - Movement */
	float ACCELERATE = 0;
	float FAST_ACCELERATE = 0;
	float DRAG_FORCE = 0;
	float MOVE_SPEED_LIMIT = 0;
	float MAX_MOVE_SPEED_LIMIT = 0;
	float DRIFT_LIMIT = 0;
	float POWER_LIMIT = 0;
	float POWER_COUNTDOWN = 0;
	float JUMP_FORCE = 0;
	float JUMP_LIMIT = 0;
	float HOVER_FORCE = 0;
	float HOVER_INTERVAL = 0;
	float KICK_INTERVAL = 0;
	float FLY_FORCE = 0;
	float FLY_INTERVAL = 0;
	float GRAVITY = 0;

	/* Stats - Action */
	float POWERUP_INTERVAL = 0;
	float INVINCIBLE_COUNTDOWN = 0;
	float DEFLECT_FORCE = 0;
	float SHELL_OFFSET = 0;
	float TAIL_ENTRY = 0;
	float TAIL_PROGRESS = 0;
	float TAIL_RECOVER = 0;
	float FIRE_ENTRY = 0;
	float FIRE_PROGRESS = 0;
	float FIRE_RECOVER = 0;
	float FIRE_OFFSETX = 0;
	float FIRE_OFFSETY = 0;
	float PIPE_MOVEMENT = 0;
	float DIE_ENTRY = 0;
	float DIE_PROGRESS = 0;

	/* Logic */
	std::string _controllerName;
	std::string _gameMasterName;
	enum class EPower
	{
		SMALL,
		LARGE,
		RACCOON,
		FIRE
	};
	EPower _power = EPower::SMALL;
	bool _invincible = false;
	bool _blink = false;
	bool _hold = false;
	bool _left = false;
	bool _blockTop = false;
	bool _ground = false;
	bool _fall = false;
	bool _fly = false;
	bool _shot = false;
	float _powerMeter = 0;
	float _powerCountdown = 0;
	float _jumpLimit = 0;
	float _hoverInterval = 0;
	float _flyInterval = 0;
	float _kickInterval = 0;
	float _powerUpInterval = 0;
	float _invincibleCountdown = 0;
	float _tailEntry = 0;
	float _tailProgress = 0;
	float _tailRecover = 0;
	float _fireEntry = 0;
	float _fireProgress = 0;
	float _fireRecover = 0;
	float _pipeMovement = 0;
	float _pipeLimit = 0;
	float _dieEntry = 0;
	float _dieProgress = 0;
	bool _diePush = false;

	/* State */
	enum class EAction
	{
		IDLE,
		WALK,
		JUMP,
		HOVER,
		RUN,
		DRIFT,
		FLY,
		CROUNCH,
		SPIN,
		SHOOT,
		KICK,
		POWERUP,
		POWERUP_TAIL,
		POWERUP_FIRE,
		POWERDOWN,
		DIE,
		PIPE_IN,
		PIPE_OUT
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
	void Hover(float elapsedMs);
	void Run(float elapsedMs);
	void Drift(float elapsedMs);
	void Fly(float elapsedMs);
	void Crounch(float elapsedMs);
	void Spin(float elapsedMs);
	void Shoot(float elapsedMs);
	void Kick(float elapsedMs);
	void PowerUp(float elapsedMs);
	void PowerTail(float elapsedMs);
	void PowerFire(float elapsedMs);
	void PowerDown(float elapsedMs);
	void Die(float elapsedMs);
	void PipeIn(float elapsedMs);
	void PipeOut(float elapsedMs);

	void HotCheat();
	void UpdateTail();
	void UpdateShell();
	void UpdatePowerBar(float elapsedMs);
	void UpdateInvincible(float elapsedMs);
	void Hit();
	void ShootFireball();


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
	void OnCollisionWithVenusFireball(pCollision e);

	void OnCollisionWithPlatform(pCollision e);
	void OnCollisionWithPipe(pCollision e);
	void OnCollisionWithTransportPipe(pCollision e);
	void OnCollisionWithBrick(pCollision e);
	void OnCollisionWithBlock(pCollision e);
	void OnCollisionWithDeadZone(pCollision e);
	void OnCollisionWithSwitch(pCollision e);

	void OnCollisionWithCoin(pCollision e);
	void OnCollisionWithExtraLife(pCollision e);
	void OnCollisionWithMushroom(pCollision e);
	void OnCollisionWithLeaf(pCollision e);
	void OnCollisionWithFlower(pCollision e);
};
typedef CMario* pMario;