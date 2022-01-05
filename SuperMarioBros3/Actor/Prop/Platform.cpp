#include "Platform.h"
#include "../../SuperMarioBros3.h"

void CPlatform::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	_solid = statsNode.attribute("solid").as_bool();

	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
}

void CPlatform::Start()
{
	_start = true;
}

void CPlatform::Update(float elapsedMs)
{
	if (_start) Start();
}

void CPlatform::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
}

int CPlatform::IsCollidable()
{
	return 1;
}

int CPlatform::IsBlocking()
{
	switch (_solid)
	{
	case true:
		return 1;
		break;

	case false:
		return 0;
		break;
	}
	return 1;
}

void CPlatform::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CPlatform::OnNoCollision(float elapsedMs)
{
}

void CPlatform::OnCollisionWith(pCollision e)
{
}
