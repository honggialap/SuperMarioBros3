#include "Engine/Game.h"

#pragma region Definitions
#pragma region Texture ID
#define TEX_CHARACTER				1
#define TEX_ENEMY					2
#define TEX_ITEM					3
#define TEX_PROPS					4
#pragma endregion

#pragma region SoundClip ID
#pragma endregion

#pragma region Actor ID
/* CONTROLLER */
#define ACT_GAMEMASTER				1001
#define ACT_INTRO_CONTROLLER		1002
#define ACT_WORLDMAP_CONTROLLER		1003
#define ACT_WORLD_CONTROLLER		1004

/* COMMONS */
#define ACT_HUD						2001
#define ACT_BACKGROUND				2002
#define ACT_SCORE					2003

/* CHARACTER */
#define ACT_MARIO					3001
#define ACT_MARIO_TAIL				3002
#define ACT_MARIO_FIREBALL			3003
#define ACT_GOOMBA					3004
#define ACT_KOOPA					3005
#define ACT_KOOPA_SENSOR			3006
#define ACT_PIRANHA					3007
#define ACT_PIRANHA_SENSOR			3008
#define ACT_VENUS					3009
#define ACT_VENUS_FIREBALL			3010
#define ACT_VENUS_SENSOR			3011

/* ITEM */
#define ACT_COIN					4001
#define ACT_FLOWER					4002
#define ACT_LEAF					4003
#define ACT_MUSHROOM				4004
#define ACT_EXTRALIFE				4005

/* PROPS */
#define ACT_BRICK					5001
#define ACT_BLOCK					5002
#define ACT_PIPE					5003
#define ACT_ENTRY_PIPE				5004
#define ACT_EXIT_PIPE				5005
#define ACT_EMPTY_PIPE				5006
#define ACT_SOLID_PLATFORM			5007
#define ACT_HOLLOWED_PLATFORM		5008
#define ACT_GOAL					5009
#define ACT_SWITCH					5010
#define ACT_SPAWNER					5011
#define ACT_DEADZONE				5012

#pragma endregion
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