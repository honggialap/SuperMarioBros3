#include "VenusFireball.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Mario.h"


void CVenusFireball::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Logic */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	DECAY_INTERVAL = statsNode.attribute("DECAY_INTERVAL").as_float();
}

void CVenusFireball::Start()
{
	_start = true;
	_animations[ANI_VENUS_FIREBALL]->Play(true);
	_decayInterval = DECAY_INTERVAL;
}

void CVenusFireball::Update(float elapsedMs)
{
	if (!_start) Start();

	_animations[ANI_VENUS_FIREBALL]->Update(elapsedMs);
	if (_decayInterval > 0) _decayInterval -= elapsedMs;
	else
	{
		_animations[ANI_VENUS_FIREBALL]->Stop();
		Destroy();
	}

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CVenusFireball::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	_animations[ANI_VENUS_FIREBALL]->Render(_x, _y);
}

int CVenusFireball::IsCollidable()
{
	return 1;
}

int CVenusFireball::IsBlocking()
{
	return 0;
}

void CVenusFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CVenusFireball::OnNoCollision(float elapsedMs)
{
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CVenusFireball::OnCollisionWith(pCollision e)
{
	if (e->_target->IsBlocking()) Destroy();

	/* Character */
	if (dynamic_cast<pMario>(e->_target))
	{
		pMario mario = dynamic_cast<pMario>(e->_target);
		mario->Hit();
		Destroy();
	}
}
