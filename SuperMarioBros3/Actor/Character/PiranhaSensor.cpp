#include "PiranhaSensor.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Mario.h"


void CPiranhaSensor::Load()
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
}

void CPiranhaSensor::Start()
{
	_start = true;
	_activate = false;
}

void CPiranhaSensor::Update(float elapsedMs)
{
	if (!_start) Start();

	_activate = false;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CPiranhaSensor::Render()
{
	if (_renderBody) _sprites[BBOX]->Render(_x, _y);
}

int CPiranhaSensor::IsCollidable()
{
	return 1;
}

int CPiranhaSensor::IsBlocking()
{
	return 0;
}

void CPiranhaSensor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CPiranhaSensor::OnNoCollision(float elapsedMs)
{
}

void CPiranhaSensor::OnCollisionWith(pCollision e)
{
	/* Character */
	if (dynamic_cast<pMario>(e->_target))
	{
		_activate = true;
	}
}
