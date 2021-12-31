#pragma once
#include "Framework/Graphics.h"

class CGameObject;
typedef CGameObject* pGameObject;

struct CSprite
{
	pGameObject _gameObject;
	int _left, _top;
	int _width, _height;
	int _offsetX, _offsetY;
	pTexture _texture;
	D3DX10_SPRITE _sprite;
	D3DXMATRIX _matScaling;

	CSprite(pGameObject gameObject, int left, int top, int width, int height, int offsetX, int offsetY, pTexture texture);
	void Render(float x, float y, bool isUI = false);
};
typedef CSprite* pSprite;