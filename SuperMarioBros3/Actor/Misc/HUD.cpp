#include "HUD.h"
#include "../../SuperMarioBros3.h"

void CHUD::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_mario = dynamic_cast<pMario>(_game->Get(statsNode.attribute("marioName").as_string()));
	POWER_LIMIT = statsNode.attribute("POWER_LIMIT").as_float();
	BLINK_INTERVAL = statsNode.attribute("BLINK_INTERVAL").as_float();

	FRAME_X = statsNode.attribute("FRAME_X").as_float();
	FRAME_Y = statsNode.attribute("FRAME_Y").as_float();
	LEVEL1_X = statsNode.attribute("LEVEL1_X").as_float();
	LEVEL1_Y = statsNode.attribute("LEVEL1_Y").as_float();
	LEVEL2_X = statsNode.attribute("LEVEL2_X").as_float();
	LEVEL2_Y = statsNode.attribute("LEVEL2_Y").as_float();
	LEVEL3_X = statsNode.attribute("LEVEL3_X").as_float();
	LEVEL3_Y = statsNode.attribute("LEVEL3_Y").as_float();
	LEVEL4_X = statsNode.attribute("LEVEL4_X").as_float();
	LEVEL4_Y = statsNode.attribute("LEVEL4_Y").as_float();
	LEVEL5_X = statsNode.attribute("LEVEL5_X").as_float();
	LEVEL5_Y = statsNode.attribute("LEVEL5_Y").as_float();
	LEVEL6_X = statsNode.attribute("LEVEL6_X").as_float();
	LEVEL6_Y = statsNode.attribute("LEVEL6_Y").as_float();
	POWER_X = statsNode.attribute("POWER_X").as_float();
	POWER_Y = statsNode.attribute("POWER_Y").as_float();
}

void CHUD::Start()
{
	_start = true;
	_blinkInterval = BLINK_INTERVAL;
}

void CHUD::Update(float elapsedMs)
{
	if (!_start) Start();
	if (_blinkInterval > 0) _blinkInterval -= elapsedMs;
	else
	{
		_blink = !_blink;
		_blinkInterval = BLINK_INTERVAL;
	}
	_level = unsigned int(_mario->_powerMeter * POWER_LIMIT / _mario->POWER_LIMIT);
}

void CHUD::Render()
{
	_sprites[SPR_HUD_FRAME]->Render(FRAME_X, FRAME_Y, true);
	if (!_mario->_fly)
	{
		if (_level > 1)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL1_X, LEVEL1_Y, true);
		if (_level > 2)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL2_X, LEVEL2_Y, true);
		if (_level > 3)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL3_X, LEVEL3_Y, true);
		if (_level > 4)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL4_X, LEVEL4_Y, true);
		if (_level > 5)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL5_X, LEVEL5_Y, true);
		if (_level > 6)	_sprites[SPR_HUD_LEVEL1]->Render(LEVEL6_X, LEVEL6_Y, true);
	}
	else
	{
		if (_blink)
		{
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL1_X, LEVEL1_Y, true);
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL2_X, LEVEL2_Y, true);
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL3_X, LEVEL3_Y, true);
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL4_X, LEVEL4_Y, true);
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL5_X, LEVEL5_Y, true);
			_sprites[SPR_HUD_LEVEL1]->Render(LEVEL6_X, LEVEL6_Y, true);
			_sprites[SPR_HUD_POWER1]->Render(POWER_X, POWER_Y, true);
		}
	}
	
}

int CHUD::IsCollidable()
{
	return 0;
}

int CHUD::IsBlocking()
{
	return 0;
}

void CHUD::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CHUD::OnNoCollision(float elapsedMs)
{
}

void CHUD::OnCollisionWith(pCollision e)
{
}
