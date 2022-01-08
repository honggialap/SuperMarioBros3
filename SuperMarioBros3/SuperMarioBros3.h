#include "Engine/Game.h"

#pragma region Actor ID
/* CONTROLLER */
#define ACT_GAMEMASTER			1001
#define ACT_WORLD				1002

/* CHARACTER */
#define ACT_MARIO				2001
#define ACT_MARIO_TAIL			2002
#define ACT_MARIO_FIREBALL		2012
#define ACT_GOOMBA				2003
#define ACT_KOOPA				2004
#define ACT_KOOPA_SENSOR		2005
#define ACT_PIRANHA				2006
#define ACT_PIRANHA_SENSOR		2007
#define ACT_VENUS				2008
#define ACT_VENUS_FIREBALL		2009
#define ACT_VENUS_SENSOR		2010

/* ITEM */
#define ACT_COIN				3001
#define ACT_EXTRALIFE			3002
#define ACT_MUSHROOM			3003
#define ACT_LEAF				3004
#define ACT_FLOWER				3005

/* PROP */
#define ACT_PLATFORM			4001
#define ACT_BRICK				4002
#define ACT_BRICK_FRAG			4012
#define ACT_BLOCK				4003
#define ACT_SPAWN_ZONE			4004
#define ACT_DEAD_ZONE			4005
#define ACT_SWITCH				4006
#define ACT_PIPE				4007
#define ACT_TRANSPORT_PIPE		4008
#define ACT_GOAL				4009

/* MISC */
#define ACT_BACKGROUND			5001
#define ACT_HUD					5002
#define ACT_SCORE_EFFECT		5003
#pragma endregion


class CSMB3 : public CGame
{
public:
	pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer, bool active
	);
};
typedef CSMB3* pSMB3;
