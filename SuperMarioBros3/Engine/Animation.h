#pragma once
#include <unordered_map>
#include <vector>

class CGameObject;
typedef CGameObject* pGameObject;

struct CAnimation
{
	pGameObject _gameObject;
	std::vector<std::pair<unsigned int, float>> _animationFrames;
	unsigned int _frameIndex = 0;
	float _elapsedTime = 0;
	bool _play = false;
	bool _loop = false;

	CAnimation(pGameObject gameObject);
	void AddFrame(unsigned int spriteId, float frameTime);
	void Play(bool loop = false);
	void Pause();
	void Stop();
	void Update(float elapsedMs);
	void Render(float x, float y, bool isUI = false);
};
typedef CAnimation* pAnimation;