#include "MarioTail.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Goomba.h"
#include "../Character/Koopa.h"
#include "../Character/Piranha.h"
#include "../Character/Venus.h"
#include "../Prop/Brick.h"
#include "../Prop/Block.h"


void CMarioTail::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	LEFT_BODY_WIDTH = bodyNode.attribute("width").as_float();
	LEFT_BODY_HEIGHT = bodyNode.attribute("height").as_float();
	LEFT_BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	LEFT_BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	bodyNode = bodyNode.next_sibling("Body");
	RIGHT_BODY_WIDTH = bodyNode.attribute("width").as_float();
	RIGHT_BODY_HEIGHT = bodyNode.attribute("height").as_float();
	RIGHT_BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	RIGHT_BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Logic */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
}

void CMarioTail::Start()
{
	_start = true;
	_left = true;
	_activate = false;
}

void CMarioTail::Update(float elapsedMs)
{
	if (!_start) Start();

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CMarioTail::Render()
{
	if (_renderBody)
	{
		if (_left) _sprites[LEFT_BBOX]->Render(_x, _y);
		else _sprites[RIGHT_BBOX]->Render(_x, _y);
	}
}

int CMarioTail::IsCollidable()
{
	if (_activate) return 1;
	else return 0;
}

int CMarioTail::IsBlocking()
{
	return 0;
}

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (_left)
	{
		left = _x + LEFT_BODY_OFFSETX - (LEFT_BODY_WIDTH / 2);
		right = _x + LEFT_BODY_OFFSETX + (LEFT_BODY_WIDTH / 2);
		top = _y + LEFT_BODY_OFFSETY + (LEFT_BODY_HEIGHT / 2);
		bottom = _y + LEFT_BODY_OFFSETY - (LEFT_BODY_HEIGHT / 2);
	}
	else
	{
		left = _x + RIGHT_BODY_OFFSETX - (RIGHT_BODY_WIDTH / 2);
		right = _x + RIGHT_BODY_OFFSETX + (RIGHT_BODY_WIDTH / 2);
		top = _y + RIGHT_BODY_OFFSETY + (RIGHT_BODY_HEIGHT / 2);
		bottom = _y + RIGHT_BODY_OFFSETY - (RIGHT_BODY_HEIGHT / 2);
	}
}

void CMarioTail::OnNoCollision(float elapsedMs)
{
}

void CMarioTail::OnCollisionWith(pCollision e)
{
	if (dynamic_cast<pGoomba>(e->_target))
		OnCollisionWithGoomba(e);

	else if (dynamic_cast<pKoopa>(e->_target))
		OnCollisionWithKoopa(e);

}

void CMarioTail::OnCollisionWithGoomba(pCollision e)
{
	pGoomba goomba = dynamic_cast<pGoomba>(e->_target);
	if (goomba->_action != CGoomba::EAction::DIE
		&& goomba->_action != CGoomba::EAction::THROWN)
	{
		goomba->HitSide();
	}
}

void CMarioTail::OnCollisionWithKoopa(pCollision e)
{
	pKoopa koopa = dynamic_cast<pKoopa>(e->_target);
	if (koopa->_action != CKoopa::EAction::THROWN)
	{
		koopa->HitByTail(_left);
	}
}
