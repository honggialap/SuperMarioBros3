#include "TransportPipe.h"
#include "../../SuperMarioBros3.h"


void CTransportPipe::Load()
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

	/* Stats */
	pugi::xml_node statsNode = prefab.child("Prefab").child("Stats");
	CAMERA_SWITCH = statsNode.attribute("CAMERA_SWITCH").as_uint();
	DESTINATION_X = statsNode.attribute("DESTINATION_X").as_float();
	DESTINATION_Y = statsNode.attribute("DESTINATION_Y").as_float();

	/* Logic */
	_renderBody = statsNode.attribute("renderBody").as_bool();
	_up = statsNode.attribute("up").as_bool();
}

void CTransportPipe::Start()
{
	_start = true;
}

void CTransportPipe::Update(float elapsedMs)
{
	if (!_start) Start();
}

void CTransportPipe::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
}

int CTransportPipe::IsCollidable()
{
	return 1;
}

int CTransportPipe::IsBlocking()
{
	return 0;
}

void CTransportPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CTransportPipe::OnNoCollision(float elapsedMs)
{
}

void CTransportPipe::OnCollisionWith(pCollision e)
{
}
