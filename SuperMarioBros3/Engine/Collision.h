#pragma once

#include <Windows.h>
#include <algorithm>
#include <vector>

#define BLOCK_PUSH_FACTOR 0.4f

class CGameObject;
typedef CGameObject* pGameObject;

struct CCollision;
typedef CCollision* pCollision;

struct CCollision
{
	pGameObject _target;			// the target object
	float _t, _nx, _ny;
	float _dx, _dy;				// *RELATIVE* movement distance between this object and obj
	bool _destroy;
	bool _overlapping;

	CCollision(
		float t, float nx, float ny, 
		float dx = 0, float dy = 0, 
		bool overlapping = false,
		pGameObject target = NULL)
	{
		_t = t;
		_nx = nx;
		_ny = ny;
		_dx = dx;
		_dy = dy;
		_overlapping = overlapping;
		_target = target;
		_destroy = false;
	}

	int WasCollided() { return _t >= 0.0f && _t <= 1.0f; }

	static bool compare(const pCollision& a, pCollision& b)
	{
		return a->_t < b->_t;
	}
};

class CCollider
{
public:
	pGameObject _gameObject;
	std::vector<pCollision> _coEvents;

	CCollider(pGameObject gameObject);

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			
		float dy,			
		float sl,			// static left
		float st,			// static top
		float sr,			// static right 
		float sb,			// static bottom
		float& t,
		float& nx,
		float& ny);

	pCollision Overlapping(
		pGameObject objDest
	);

	void ScanOverlap(
		std::vector<pGameObject>* objDests
	);

	pCollision SweptAABB(
		float elapsedMs,
		pGameObject objDest
	);

	void Scan(
		float elapsedMs,
		std::vector<pGameObject>* objDests
	);

	void Filter(
		pCollision& colX,
		pCollision& colY,
		int filterBlock,
		int filterX,
		int filterY
	);

	void Process(
		float elapsedMs,
		std::vector<pGameObject>* coObjects
	);
};
typedef CCollider* pCollider;
