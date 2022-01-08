#include "BrickFrag.h"
#include "../../SuperMarioBros3.h"

void CBrickFrag::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Logic */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
	DRAG_FORCE = statsNode.attribute("DRAG_FORCE").as_float();
	GRAVITY = statsNode.attribute("GRAVITY").as_float();
	INITIAL_FORCE = statsNode.attribute("INITIAL_FORCE").as_float();
}

void CBrickFrag::Start()
{
	_start = true;
	_decayInterval = DECAY_INTERVAL;
	if (_left) _vx = -INITIAL_FORCE;
	else _vx = INITIAL_FORCE;
}

void CBrickFrag::Update(float elapsedMs)
{
	if (!_start) Start();

	_vy -= GRAVITY * elapsedMs;
	_vx = _vx / DRAG_FORCE;
	_vy = _vy / DRAG_FORCE;

	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;

	if (_decayInterval > 0) _decayInterval -= elapsedMs;
	else
	{
		Destroy();
	}
}

void CBrickFrag::Render()
{
	if(_left)	_sprites[SPR_BRICK_FRAG_LEFT]->Render(_x, _y);
	else _sprites[SPR_BRICK_FRAG_LEFT]->Render(_x, _y);
}

int CBrickFrag::IsCollidable()
{
	return 0;
}

int CBrickFrag::IsBlocking()
{
	return 0;
}

void CBrickFrag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CBrickFrag::OnNoCollision(float elapsedMs)
{
}

void CBrickFrag::OnCollisionWith(pCollision e)
{
}