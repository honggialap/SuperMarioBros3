#include "Animation.h"
#include "Sprite.h"
#include "GameObject.h"

CAnimation::CAnimation(pGameObject gameObject)
{
	_gameObject = gameObject;
}

void CAnimation::AddFrame(unsigned int spriteId, float frameTime)
{
	_animationFrames.push_back(std::make_pair(spriteId, frameTime));
}

void CAnimation::Play(bool loop)
{
	_play = true;
	_loop = loop;
	_frameIndex = 0;
	_elapsedTime = 0;
}

void CAnimation::Pause()
{
	_play = false;
}

void CAnimation::Stop()
{
	_play = false;
	_loop = false;
	_frameIndex = 0;
	_elapsedTime = 0;
}

void CAnimation::Update(float elapsedMs)
{
	if (_play)
	{
		_elapsedTime += elapsedMs;
		while (_elapsedTime >= _animationFrames.at(_frameIndex).second)
		{
			_elapsedTime -= _animationFrames.at(_frameIndex).second;
			_frameIndex++;
			if (_frameIndex >= _animationFrames.size())
			{
				if (_loop) _frameIndex = 0;
				else Stop();
			}
		}
	}
}

void CAnimation::Render(float x, float y, bool isUI)
{
	_gameObject->_sprites[
		_animationFrames.at(_frameIndex).first
	]->Render(x, y, isUI);
}
