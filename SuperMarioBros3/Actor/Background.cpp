#include "Background.h"
#include "../Engine/Game.h"
#include "../Engine/Library/pugixml.hpp"


void CBackground::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Tile map */
	pugi::xml_node tileMapNode = prefab.child("Prefab").child("TileMap");
	_mapWidth = tileMapNode.attribute("mapWidth").as_uint();
	_mapHeight = tileMapNode.attribute("mapHeight").as_uint();
	_cellWidth = tileMapNode.attribute("cellWidth").as_float();
	_cellHeight = tileMapNode.attribute("cellHeight").as_float();
	for (auto cellNode = tileMapNode.child("Cell");
		cellNode;
		cellNode = cellNode.next_sibling("Cell"))
	{
		_cells.push_back(
			cellNode.attribute("spriteId").as_uint()
		);
	}
}

void CBackground::Start()
{
}

void CBackground::Update(float elapsedMs)
{
}

void CBackground::Render()
{
	float boundLeft = _game->cameraX - BUFFER_CAM;
	float boundTop = _game->cameraY + BUFFER_CAM;
	float boundRight = _game->cameraX + _game->_graphics->_backBufferWidth + BUFFER_CAM;
	float boundBottom = _game->cameraY - _game->_graphics->_backBufferHeight - BUFFER_CAM;

	for (unsigned int i = 0; i < _cells.size(); i++)
	{
		float renderX = _x + (i % _mapWidth) * _cellWidth;
		float renderY = _y - (i / _mapWidth) * _cellHeight;
		if (renderX > boundLeft
			&& renderX < boundRight
			&& renderY > boundBottom
			&& renderY < boundTop)
		{
			_sprites[_cells.at(i)]->Render(renderX, renderY);
		}
	}
}

void CBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
