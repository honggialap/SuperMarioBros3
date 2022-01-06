#include "Pipe.h"
#include "../../SuperMarioBros3.h"


void CPipe::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	_renderBody = statsNode.attribute("renderBody").as_bool();
	_green = statsNode.attribute("green").as_bool();
	_up = statsNode.attribute("up").as_bool();

	pugi::xml_node bodyNode = prefab.child("Prefab").child("Body");
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();
}

void CPipe::Start()
{
	_start = true;
}

void CPipe::Update(float elapsedMs)
{
	if (_start) Start();
}

void CPipe::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
	if (_green)
	{
		if (_up) _sprites[SPR_PIPE_GREEN_UP]->Render(_x, _y);
		else _sprites[SPR_PIPE_GREEN_DOWN]->Render(_x, _y);
	}
	else
	{
		if (_up) _sprites[SPR_PIPE_BLACK_UP]->Render(_x, _y);
		else _sprites[SPR_PIPE_BLACK_DOWN]->Render(_x, _y);
	}

}

int CPipe::IsCollidable()
{
	return 1;
}

int CPipe::IsBlocking()
{
	return 0;
}

void CPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CPipe::OnNoCollision(float elapsedMs)
{
}

void CPipe::OnCollisionWith(pCollision e)
{
}
