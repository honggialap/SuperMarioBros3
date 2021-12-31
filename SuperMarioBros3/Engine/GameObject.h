#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "Collision.h"

#include <string>


class CGame;
typedef CGame* pGame;


struct CScene;
typedef CScene* pScene;


class CGameObject
{
public:
	pGame _game = nullptr;
	pScene _scene = nullptr;
	unsigned int _id = 0;
	std::string _name;
	std::string _source;
	CGameObject(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string prefabSource,
		float posX, float posY, int gridX, int gridY, unsigned int layer
	);
	~CGameObject();
	virtual void Load();

	bool _active = false;
	void Active();
	void Deactive();

	bool _start = false;
	virtual void Start() = 0;
	virtual void Update(float elapsedMs) = 0;
	virtual void Render() = 0;

	bool _destroy = false;
	bool IsDestroyed() { return _destroy; }
	void Destroy();

	float _x = 0;
	float _y = 0;
	int _gridX = 0;
	int _gridY = 0;
	void SetPosition(float x, float y) { _x = x; _y = y; }
	void GetPosition(float& x, float& y) { x = _x; y = _y; }

	float _vx = 0;
	float _vy = 0;
	void SetSpeed(float vx, float vy) { _vx = vx; _vy = vy; }
	void AddSpeed(float vx, float vy) { _vx += vx; _vy += vy; }
	void GetSpeed(float &vx, float &vy) { vx = _vx; vy = _vy; }

	float _ax = 0;
	float _ay = 0;
	void SetAccelerate(float ax, float ay) { _ax = ax; _ay = ay; }
	void AddAccelerate(float ax, float ay) { _ax += ax; _ay += ay; }
	void GetAccelerate(float& ax, float& ay) { ax = _ax; ay = _ay; }

	std::unordered_map<unsigned int, pSprite> _sprites;
	void AddSprite(unsigned int id, pSprite sprite) { _sprites[id] = sprite; }
	pSprite GetSprite(unsigned int id) { return _sprites[id]; }

	std::unordered_map<unsigned int, pAnimation> _animations;
	void AddAnimation(unsigned int id, pAnimation animation) { _animations[id] = animation; }
	pAnimation GetAnimatino(unsigned int id) { return _animations[id]; }

	pCollider _collider;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual int IsCollidable() { return 0; };
	virtual void OnNoCollision(float dt) {};
	virtual void OnCollisionWith(pCollision e) {};
	virtual int IsBlocking() { return 1; }


	unsigned int _layer = 0;
	static bool Compare(CGameObject* a, CGameObject* b)
	{
		return a->_layer < b->_layer;
	}
};
typedef CGameObject* pGameObject;