#pragma once
#include "../../../../Engine/GameObject.h"
#include "VenusFireball.h"
#include "VenusSensor.h"

#pragma region Sprite ID
#define BBOX								9991

#define SPR_VENUS_UP1_LEFT					1001
#define SPR_VENUS_UP2_LEFT					1002
#define SPR_VENUS_DOWN1_LEFT				1003
#define SPR_VENUS_DOWN2_LEFT				1004
#define SPR_VENUS_UP1_RIGHT					1101
#define SPR_VENUS_UP2_RIGHT					1102
#define SPR_VENUS_DOWN1_RIGHT				1103
#define SPR_VENUS_DOWN2_RIGHT				1104
#define SPR_PIRANHA1						1201
#define SPR_PIRANHA2						1202

#define SPR_RED_VENUS_UP1_LEFT				2001
#define SPR_RED_VENUS_UP2_LEFT				2002
#define SPR_RED_VENUS_DOWN1_LEFT			2003
#define SPR_RED_VENUS_DOWN2_LEFT			2004
#define SPR_RED_VENUS_UP1_RIGHT				2101
#define SPR_RED_VENUS_UP2_RIGHT				2102
#define SPR_RED_VENUS_DOWN1_RIGHT			2103
#define SPR_RED_VENUS_DOWN2_RIGHT			2104
#define SPR_RED_PIRANHA1					2201
#define SPR_RED_PIRANHA2					2202
#pragma endregion

#pragma region Animation ID
#define ANI_VENUS_UP_LEFT					1001
#define ANI_VENUS_DOWN_LEFT					1002
#define ANI_VENUS_UP_RIGHT					1101	
#define ANI_VENUS_DOWN_RIGHT				1102
#define ANI_PIRANHA							1201

#define ANI_RED_VENUS_UP_LEFT				2001
#define ANI_RED_VENUS_DOWN_LEFT				2002
#define ANI_RED_VENUS_UP_RIGHT				2101
#define ANI_RED_VENUS_DOWN_RIGHT			2102
#define ANI_RED_PIRANHA						2201
#pragma endregion


class CVenus : public CGameObject
{
public:
	CVenus(
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
	pVenusFireball _fireball = nullptr;
	pVenusSensor _sensor = nullptr;

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Stats */
	float HIDE_INTERVAL = 0;
	float STAND_INTERVAL = 0;
	float SHOOT_INTERVAL = 0;
	float GROW_LIMIT = 0;
	float BORROW_LIMIT = 0;

	/* Logic */
	enum class EType
	{
		VENUS,
		RED_VENUS
	};
	EType _type = EType::VENUS;
	bool _left = false;
	bool _up = false;
	float _currentHideInterval = 0;
	float _currentStandInterval = 0;
	float _shootStandInterval = 0;

	/* State */
	enum class EAction
	{
		HIDE,
		GROW,
		STAND,
		BURROW,
		SHOOT,
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
	void Shoot(float elapsedMs);
	void Die(float elapsedMs);

	/* Collision */
	int IsCollidable();
	int IsBlocking();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision e);
};
typedef CVenus* pVenus;