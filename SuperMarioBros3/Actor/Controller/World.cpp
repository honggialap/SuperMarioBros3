#include "World.h"
#include "../../SuperMarioBros3.h"

void CWorld::Load()
{
	CGameObject::Load();
}

void CWorld::Start()
{
	_start = true;
	SetCamera(0);
}

void CWorld::Update(float elapsedMs)
{
	if (!_start) Start();
	if (_camTarget != nullptr)
	{
		float centerX = _game->cameraX + (_game->_graphics->_backBufferWidth/2);
		float centerY = _game->cameraY - (_game->_graphics->_backBufferHeight/2);
		float trackLeft = centerX + _camTrackOffsetX - (_camTrackWidth / 2);
		float trackTop = centerY + _camTrackOffsetY + (_camTrackHeight / 2);
		float trackRight = centerX + _camTrackOffsetX + (_camTrackWidth / 2);
		float trackBottom = centerY + _camTrackOffsetY - (_camTrackHeight / 2);

		if (_camTarget->_x < trackLeft)
		{
			_game->MoveCameraBy(_camTarget->_x - trackLeft, 0);
		}
		else if (_camTarget->_x > trackRight)
		{
			_game->MoveCameraBy(_camTarget->_x - trackRight, 0);
		}

		if (_camTarget->_y > trackTop)
		{
			_game->MoveCameraBy(0, _camTarget->_y - trackTop);
		}
		else if (_camTarget->_y < trackBottom)
		{
			_game->MoveCameraBy(0, _camTarget->_y - trackBottom);
		}

		float camX = _game->cameraX;
		float camY = _game->cameraY;

		if (camX < _camBoundLeft)
		{
			camX = _camBoundLeft;
		}
		else if (camX > _camBoundRight)
		{
			camX = _camBoundRight;
		}

		if (camY > _camBoundTop)
		{
			camY = _camBoundTop;
		}
		else if (camY < _camBoundBottom)
		{
			camY = _camBoundBottom;
		}
		_game->MoveCameraTo(camX, camY);
	}
}

void CWorld::Render()
{
}

void CWorld::SetCamera(unsigned int id)
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	for (auto cameraNode = prefab.child("Prefab").child("Stats").child("Camera");
		cameraNode;
		cameraNode = cameraNode.next_sibling("Camera"))
	{
		if (id == cameraNode.attribute("id").as_uint())
		{
			_camBoundLeft = cameraNode.attribute("leftBound").as_float();
			_camBoundTop = cameraNode.attribute("topBound").as_float();
			_camBoundRight = cameraNode.attribute("rightBound").as_float();
			_camBoundBottom = cameraNode.attribute("bottomBound").as_float();
			_camTrackWidth = cameraNode.attribute("trackWidth").as_float();
			_camTrackHeight = cameraNode.attribute("trackHeight").as_float();
			_camTrackOffsetX = cameraNode.attribute("trackOffsetX").as_float();
			_camTrackOffsetY = cameraNode.attribute("trackOffsetY").as_float();
			_camTarget = _game->Get(cameraNode.attribute("targetName").as_string());
		}
	}
}

void CWorld::HandleAction(float elapsedMs)
{
}
