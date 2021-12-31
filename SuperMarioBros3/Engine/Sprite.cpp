#include "Sprite.h"
#include "GameObject.h"
#include "Game.h"

#include "Framework/Debug.h"

CSprite::CSprite(pGameObject gameObject, int left, int top, int width, int height, int offsetX, int offsetY, pTexture texture)
{
	_gameObject = gameObject;
	_left = left;
	_top = top;
	_width = width;
	_height = height;
	_offsetX = offsetX;
	_offsetY = offsetY;
	_texture = texture;

	float texWidth = (float)_texture->_width;
	float texHeight = (float)_texture->_height;

	// Set the sprite’s shader resource view
	_sprite.pTexture = _texture->_rsview;
	_sprite.TexCoord.x = _left / texWidth;
	_sprite.TexCoord.y = _top / texHeight;
	_sprite.TexSize.x = _width / texWidth;
	_sprite.TexSize.y = _height / texHeight;
	_sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_sprite.TextureIndex = 0;

	D3DXMatrixScaling(&_matScaling, (FLOAT)_width, (FLOAT)_height, 1.0f);
}

void CSprite::Render(float x, float y, bool isUI)
{
	auto graphics = _gameObject->_game->_graphics;
	float cameraX = (FLOAT)floor(_gameObject->_game->cameraX);
	float cameraY = (FLOAT)floor(_gameObject->_game->cameraY);

	D3DXMATRIX matTranslation;

	x = (FLOAT)floor(x) + _offsetX;
	y = (FLOAT)floor(y) + _offsetY;

	if (isUI)
		D3DXMatrixTranslation(&matTranslation, x, graphics->_backBufferHeight + y, 0.1f);
	else
		D3DXMatrixTranslation(&matTranslation, x - cameraX, graphics->_backBufferHeight + y - cameraY, 0.1f);

	_sprite.matWorld = (_matScaling * matTranslation);

	graphics->_spriteHandler->DrawSpritesImmediate(&_sprite, 1, 0, 0);

}
