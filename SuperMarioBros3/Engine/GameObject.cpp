#include "GameObject.h"
#include "Game.h"

CGameObject::CGameObject(
	pGame game, pScene scene,
	unsigned int id, std::string name, std::string prefabSource,
	float posX, float posY, int gridX, int gridY, unsigned int layer
)
{
	_game = game;
	_scene = scene;
	_id = id;
	_name = name;
	_source = prefabSource;
	_x = posX;
	_y = posY;
	_gridX = gridX;
	_gridY = gridY;
	_layer = layer;
	_destroy = false;
	_collider = new CCollider(this);
}


CGameObject::~CGameObject()
{
	{
		delete _collider;
		_collider = nullptr;
	}

	for (auto& animation : _animations)
	{
		delete animation.second;
		animation.second = nullptr;
	}

	for (auto& sprite : _sprites)
	{
		delete sprite.second;
		sprite.second = nullptr;
	}
}


void CGameObject::Load()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());


	/* Sprite */
	pugi::xml_node spritesNode = prefab.child("Prefab").child("Sprites");
	for (auto spriteNode = spritesNode.child("Sprite");
		spriteNode;
		spriteNode = spriteNode.next_sibling("Sprite"))
	{
		AddSprite(
			spriteNode.attribute("id").as_uint(),
			new CSprite(
				this,
				spriteNode.attribute("left").as_uint(),
				spriteNode.attribute("top").as_uint(),
				spriteNode.attribute("width").as_uint(),
				spriteNode.attribute("height").as_uint(),
				spriteNode.attribute("offsetX").as_int(),
				spriteNode.attribute("offsetY").as_int(),
				_game->_graphics->_textures[
					spriteNode.attribute("textureId").as_uint()
				]
			)
		);
	}


	/* Animation */
	pugi::xml_node animationsNode = prefab.child("Prefab").child("Animations");
	for (auto animationNode = animationsNode.child("Animation");
		animationNode;
		animationNode = animationNode.next_sibling("Animation"))
	{
		auto animation = new CAnimation(this);
		AddAnimation(
			animationNode.attribute("id").as_uint(),
			animation
		);

		for (auto frameNode = animationNode.child("Frame");
			frameNode;
			frameNode = frameNode.next_sibling("Frame"))
		{
			animation->AddFrame(
				frameNode.attribute("spriteId").as_uint(),
				frameNode.attribute("time").as_float()
			);
		}
	}
}

void CGameObject::Active()
{
	if (!_active)
	{
		_active = true;
		_game->Activate(_id);
	}
}


void CGameObject::Deactive()
{
	if (_active)
	{
		_active = false;
		_game->Deactivate(_id);
	}
}


void CGameObject::Destroy()
{
	_destroy = true;
	Deactive();
}