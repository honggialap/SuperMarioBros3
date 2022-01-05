#include "KoopaSensor.h"
#include "../../SuperMarioBros3.h"

void CKoopaSensor::Load()
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

void CKoopaSensor::Start()
{
	_start = true;
	_left = true;
}

void CKoopaSensor::Update(float elapsedMs)
{
	if (!_start) Start();

	_activate = true;

	std::vector<pGameObject> colidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &colidables);
}

void CKoopaSensor::Render()
{
	if (_renderBody)
	{
		if (_left) _sprites[LEFT_BBOX]->Render(_x, _y);
		else _sprites[RIGHT_BBOX]->Render(_x, _y);
	}		
}

int CKoopaSensor::IsCollidable()
{
	return 1;
}

int CKoopaSensor::IsBlocking()
{
	return 0;
}

void CKoopaSensor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CKoopaSensor::OnNoCollision(float elapsedMs)
{
}

void CKoopaSensor::OnCollisionWith(pCollision e)
{
	_activate = false;
}
