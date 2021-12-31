#include "SolidPlatform.h"
#include "../../../../Engine/Game.h"
#include "../../../../Engine/Library/pugixml.hpp"

void CSolidPlatform::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */	
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
}

void CSolidPlatform::Start()
{
	_start = true;
}

void CSolidPlatform::Update(float elapsedMs)
{
	if (_start) Start();
}

void CSolidPlatform::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
}

int CSolidPlatform::IsCollidable()
{
	return 1;
}

int CSolidPlatform::IsBlocking()
{
	return 1;
}

void CSolidPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CSolidPlatform::OnNoCollision(float elapsedMs)
{
}

void CSolidPlatform::OnCollisionWith(pCollision e)
{
}
