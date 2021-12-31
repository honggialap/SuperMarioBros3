#pragma once
#include "Framework/Application.h"
#include "Framework/Graphics.h"
#include "Framework/Input.h"
#include "Framework/Time.h"
#include "GameObject.h"

#include <algorithm>
#include <map>


struct CScene
{
	bool _load;
	bool _play;
	std::string _source;
	std::vector<unsigned int> _gameObjects;

	CScene(std::string source)
	{
		_source = source;
		_load = false;
		_play = false;
	}
};
typedef CScene* pScene;


class CGame : public CKeyHandler
{
public:
	/* Framework */
	unsigned int _fps = 0;
	pApplication _application;
	pTime _time;
	pGraphics _graphics;
	pInput _input;

	CGame();
	~CGame();
	void Load(HINSTANCE hInstance, std::string gameDataPath);
	void Run();
	void Update(float elapsedMs);
	void Render();
	void Shutdown();


	/* Player */
	float cameraX = 0;
	float cameraY = 0;
	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

	void MoveCameraTo(float x, float y);
	void MoveCameraBy(float dx, float dy);
	void Bind(int keyCode);
	void KeyState();
	void OnKeyUp(int keyCode);
	void OnKeyDown(int keyCode);
	bool IsDown(int keyCode);
	bool IsPressed(int keyCode);
	bool IsReleased(int keyCode);


	/* Scene */
	std::map<unsigned int, pScene> _scenes;
	
	void PlayScene(unsigned int sceneID);
	void StopScene(unsigned int sceneID);
	void LoadScene();


	/* Game Object */
	unsigned int nextGameObjectId = 0;
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;
	std::vector<unsigned int> _addedQueue;
	std::vector<unsigned int> _removedQueue;
	std::vector<unsigned int> _updateQueue;
	std::vector<pGameObject> _renderQueue;

	virtual pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string prefabSource, 
		float posX, float posY, int gridX, int gridY,
		unsigned int layer, bool active
	) = 0;
	void Add(pGameObject gameObject);
	void Purge();
	void Activate(unsigned int gameObjectId);
	void Deactivate(unsigned int gameObjectId);
	pGameObject Get(unsigned int gameObjectId);
	pGameObject Get(std::string gameObjectName);

	int _gridWidth = 0;
	int _gridHeight = 0;
	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;
	void AddGrid(unsigned int gameObjectId);
	void RemoveGrid(unsigned int gameObjectId);
	void UpdateGrid(unsigned int gameObjectId);
	std::vector<pGameObject> GetLocal(unsigned int gameObjectId);
};
typedef CGame* pGame;