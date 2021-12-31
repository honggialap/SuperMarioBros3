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


#pragma endregion
#pragma endregion


class CGameSample : public CGame
{
public:
	pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer, bool active
	);
};
typedef CGameSample* pGameSample;