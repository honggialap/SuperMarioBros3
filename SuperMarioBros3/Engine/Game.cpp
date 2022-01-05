#include "Game.h"


CGame::CGame()
{
    _application = new CApplication();
    _time = new CTime();
    _graphics = new CGraphics();
    _input = new CInput();
}


CGame::~CGame()
{
    delete _input;
    _input = nullptr;

    delete _graphics;
    _graphics = nullptr;

    delete _time;
    _time = nullptr;

    delete _application;
    _application = nullptr;
}


#pragma region Framework
void CGame::Load(HINSTANCE hInstance, std::string gameDataPath)
{
    /* Read file */
    pugi::xml_document gameData;
    gameData.load_file(gameDataPath.c_str());


    /* Game Settings */
    pugi::xml_node gameSettingsNode = gameData.child("GameData").child("GameSettings");
    _fps = gameSettingsNode.attribute("fps").as_uint();
    _gridWidth = gameSettingsNode.attribute("gridWidth").as_int();
    _gridHeight = gameSettingsNode.attribute("gridHeight").as_int();

    /* Game Player Settings */
    pugi::xml_node playerSettingsNode = gameData.child("GameData").child("PlayerSettings");
    for (auto buttonNode = playerSettingsNode.child("Button");
        buttonNode;
        buttonNode = buttonNode.next_sibling("Button"))
    {
        Bind(buttonNode.attribute("keyCode").as_int());
    }


    /* Framework layer initialize */
    _application->Create(
        hInstance,
        std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
            gameSettingsNode.attribute("gameTitle").as_string()
        ),
        gameSettingsNode.attribute("resolutionWidth").as_uint(),
        gameSettingsNode.attribute("resolutionHeight").as_uint()
    );

    _graphics->Initialize(_application->_hWnd);

    _input->Initialize(_application->_hInstance, _application->_hWnd, this);


    /* Texture loading */
    pugi::xml_node texturesNode = gameData.child("GameData").child("Textures");
    for (auto textureNode = texturesNode.child("Texture");
        textureNode;
        textureNode = textureNode.next_sibling("Texture"))
    {
        _graphics->LoadTexture(
            textureNode.attribute("id").as_uint(),
            std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
                textureNode.attribute("source").as_string()
            )
        );
    }


    /* Scene list loading */
    pugi::xml_node scenesNode = gameData.child("GameData").child("Scenes");
    for (
        auto sceneNode = scenesNode.child("Scene");
        sceneNode;
        sceneNode = sceneNode.next_sibling("Scene")
        )
    {
        auto scene = new CScene(sceneNode.attribute("source").as_string());
        _scenes[sceneNode.attribute("id").as_uint()] = scene;
    }
}


void CGame::Run()
{
    float msPerFrame = 1000.0f / _fps;
    float elapsedMs = 0;
    bool done = false;

    PlayScene(0);
    _time->Start();
    while (!done)
    {
        _time->Tick();
        elapsedMs += _time->_elapsedMs;

        if (elapsedMs >= msPerFrame)
        {
            _input->Process();
            Update(elapsedMs);
            Render();
            elapsedMs = 0;
        }
        else
        {
            Sleep(DWORD(msPerFrame - elapsedMs));
        }

        LoadScene();
        done = _application->HandleMessage();
    }
    Shutdown();
}


void CGame::Update(float elapsedMs)
{
    /* Active & Deactive */
    for (auto gameObject : _removedQueue)
    {
        _updateQueue.erase(
            std::remove(_updateQueue.begin(), _updateQueue.end(), gameObject)
        );
    }
    _removedQueue.clear();

    for (auto gameObject : _addedQueue)
    {
        _updateQueue.push_back(gameObject);
    }
    _addedQueue.clear();

    /* Destroy */
    Purge();

    /* Update & Render */
    _renderQueue.clear();
    for (auto gameObject : _updateQueue)
    {
        _gameObjects[gameObject]->Update(elapsedMs);
        UpdateGrid(gameObject);
        _renderQueue.push_back(_gameObjects[gameObject]);
    }
    std::sort(_renderQueue.begin(), _renderQueue.end(), CGameObject::Compare);
}


void CGame::Render()
{
    _graphics->_device->ClearRenderTargetView(
        _graphics->_renderTargetView,
        D3DXCOLOR(200.0f / 255, 200.0f / 255, 255.0f / 255, 0.0f)
    );

    _graphics->_spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);
    FLOAT NewBlendFactor[4] = { 0,0,0,0 };
    _graphics->_device->OMSetBlendState(_graphics->_blendStateAlpha, NewBlendFactor, 0xffffffff);

    for (auto& gameObject : _renderQueue)
    {
        gameObject->Render();
    }

    _graphics->_spriteHandler->End();
    _graphics->_swapChain->Present(0, 0);
}


void CGame::Shutdown()
{
    for (auto gameObject : _gameObjects)
    {
        delete gameObject.second;
        gameObject.second = nullptr;
    }
    _gameObjects.clear();
    _input->Shutdown();
    _graphics->Shutdown();
}
#pragma endregion


#pragma region Player
void CGame::MoveCameraTo(float x, float y)
{
    cameraX = x;
    cameraY = y;
}


void CGame::MoveCameraBy(float dx, float dy)
{
    cameraX += dx;
    cameraY += dy;
}


void CGame::Bind(int keyCode)
{
    _previousButtonState[keyCode] = false;
    _currentButtonState[keyCode] = false;
}


void CGame::KeyState()
{
    _previousButtonState = _currentButtonState;
}


void CGame::OnKeyUp(int keyCode)
{
    _currentButtonState[keyCode] = false;
}


void CGame::OnKeyDown(int keyCode)
{
    _currentButtonState[keyCode] = true;
}


bool CGame::IsDown(int keyCode)
{
    return _currentButtonState[keyCode];
}


bool CGame::IsPressed(int keyCode)
{
    return !_previousButtonState[keyCode] && _currentButtonState[keyCode];
}


bool CGame::IsReleased(int keyCode)
{
    return _previousButtonState[keyCode] && !_currentButtonState[keyCode];
}
#pragma endregion


#pragma region Scene
void CGame::PlayScene(unsigned int sceneID)
{
    auto it = _scenes.find(sceneID);
    if (it != _scenes.end())
    {
        auto scene = it->second;
        if (!scene->_play && !scene->_load)
        {
            scene->_play = true;
        }
    }
}


void CGame::StopScene(unsigned int sceneID)
{
    auto it = _scenes.find(sceneID);
    if (it != _scenes.end())
    {
        auto scene = it->second;
        if (scene->_play && scene->_load)
        {
            scene->_play = false;
        }
    }
}


void CGame::LoadScene()
{
    for (auto& it : _scenes)
    {
        auto scene = it.second;

        // Load
        if (scene->_play && !scene->_load)
        {
            /* Read file */
            pugi::xml_document sceneDoc;
            sceneDoc.load_file(scene->_source.c_str());
            
            /* Create */
            for (auto gameObjectNode = sceneDoc.child("Scene").child("GameObject");
                gameObjectNode;
                gameObjectNode = gameObjectNode.next_sibling("GameObject"))
            {
                Create(
                    scene,
                    gameObjectNode.attribute("actor").as_uint(),
                    gameObjectNode.attribute("name").as_string(),
                    gameObjectNode.attribute("source").as_string(),
                    gameObjectNode.attribute("x").as_float(),
                    gameObjectNode.attribute("y").as_float(),
                    gameObjectNode.attribute("gridX").as_int(),
                    gameObjectNode.attribute("gridY").as_int(),
                    gameObjectNode.attribute("layer").as_uint(),
                    gameObjectNode.attribute("active").as_bool()
                    );
            }
            scene->_load = true;
        }

        // Unload
        else if (!scene->_play && scene->_load)
        {
            for (auto gameObject : scene->_gameObjects)
            {
                _gameObjects[gameObject]->Destroy();
            }
            scene->_load = false;
        }
    }
}
#pragma endregion


#pragma region GameObject
void CGame::Add(pGameObject gameObject)
{
    _gameObjects[gameObject->_id] = gameObject;
    _dictionary[gameObject->_name] = gameObject->_id;
    gameObject->_scene->_gameObjects.push_back(gameObject->_id);
}


void CGame::Purge()
{
    auto it = _gameObjects.begin();
    while (it != _gameObjects.end())
    {
        if (it->second->_destroy)
        {
            it->second->_scene->_gameObjects.erase(
                std::remove(
                    it->second->_scene->_gameObjects.begin(),
                    it->second->_scene->_gameObjects.end(),
                    8),
                it->second->_scene->_gameObjects.end());
            _dictionary.erase(it->second->_name);
            delete it->second;
            it->second = nullptr;
            it = _gameObjects.erase(it);
        }
        else
        {
            it++;
        }
    }
}


void CGame::Activate(unsigned int gameObjectId)
{
    _addedQueue.push_back(gameObjectId);
    AddGrid(gameObjectId);
}


void CGame::Deactivate(unsigned int gameObjectId)
{
    _removedQueue.push_back(gameObjectId);
    RemoveGrid(gameObjectId);
}


pGameObject CGame::Get(unsigned int gameObjectId)
{
    if (_gameObjects.find(gameObjectId) == _gameObjects.end())
        return nullptr;
    else
        return _gameObjects[gameObjectId];
}


pGameObject CGame::Get(std::string gameObjectName)
{
    if (_dictionary.find(gameObjectName) == _dictionary.end())
        return nullptr;
    else
        return Get(_dictionary[gameObjectName]);
}


void CGame::AddGrid(unsigned int gameObjectId)
{
    int gridX = _gameObjects[gameObjectId]->_gridX;
    int gridY = _gameObjects[gameObjectId]->_gridY;
    auto cell = std::make_pair(gridX, gridY);

    auto it = _grid.find(cell);
    if (it == _grid.end())
    {
        _grid[cell] = { gameObjectId };
    }
    else
    {
        _grid[cell].push_back(gameObjectId);
    }
}


void CGame::RemoveGrid(unsigned int gameObjectId)
{
    int gridX = _gameObjects[gameObjectId]->_gridX;
    int gridY = _gameObjects[gameObjectId]->_gridY;
    auto cell = std::make_pair(gridX, gridY);
    std::vector<unsigned int>& it = _grid.find(cell)->second;
    it.erase(
        std::remove(it.begin(), it.end(), gameObjectId),
        it.end()
    );
}


void CGame::UpdateGrid(unsigned int gameObjectId)
{
    if (_gameObjects[gameObjectId]->_active)
    {
        float currentPosX = _gameObjects[gameObjectId]->_x;
        int currentGridX = _gameObjects[gameObjectId]->_gridX;
        int newGridX = 0;
        if (int(currentPosX) > (_gridWidth / 2))
        {
            newGridX = (int(currentPosX - (_gridWidth / 2)) / _gridWidth) + 1;
        }
        else if (int(currentPosX) < (-_gridWidth / 2))
        {
            newGridX = (int(currentPosX + (_gridWidth / 2)) / _gridWidth) - 1;
        }

        float currentPosY = _gameObjects[gameObjectId]->_y;
        int currentGridY = _gameObjects[gameObjectId]->_gridY;
        int newGridY = 0;
        if (int(currentPosY) > (_gridHeight / 2))
        {
            newGridY = (int(currentPosY - (_gridHeight / 2)) / _gridHeight) + 1;
        }
        else if (int(currentPosY) < (-_gridHeight / 2))
        {
            newGridY = (int(currentPosY + (_gridHeight / 2)) / _gridHeight) - 1;
        }

        if (newGridX != currentGridX || newGridY != currentGridY)
        {
            RemoveGrid(gameObjectId);
            _gameObjects[gameObjectId]->_gridX = newGridX;
            _gameObjects[gameObjectId]->_gridY = newGridY;
            AddGrid(gameObjectId);
        }
    }
}


std::vector<pGameObject> CGame::GetLocal(unsigned int gameObjectId)
{
    std::vector<unsigned int> local;
    std::vector<pGameObject> gameObjects;

    int gridX = _gameObjects[gameObjectId]->_gridX;
    int gridY = _gameObjects[gameObjectId]->_gridY;

    /* left-top */
    auto cell = std::make_pair(gridX - 1, gridY + 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* top */
    cell = std::make_pair(gridX, gridY + 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* right-top */
    cell = std::make_pair(gridX + 1, gridY + 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* left */
    cell = std::make_pair(gridX - 1, gridY);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* center */
    cell = std::make_pair(gridX, gridY);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* right */
    cell = std::make_pair(gridX + 1, gridY);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* left-bottom */
    cell = std::make_pair(gridX - 1, gridY - 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* bottom */
    cell = std::make_pair(gridX, gridY - 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    /* right-bottom */
    cell = std::make_pair(gridX + 1, gridY - 1);
    for (auto gameObject : _grid[cell])
    {
        local.push_back(gameObject);
    }

    for (auto gameObject : local)
    {
        if (gameObject != gameObjectId)
        {
            gameObjects.push_back(_gameObjects[gameObject]);
        }
    }

    return gameObjects;
}
#pragma endregion