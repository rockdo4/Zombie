#include "stdafx.h"
#include "Framework.h"
#include "SpriteGo.h"

void Framework::Init(int width, int height, const std::string& name)
{
    FONT_MGR.Load("fonts/DS-DIGI.ttf");

	window.create(sf::VideoMode(width, height), name);
    DATATABLE_MGR.Init();
    Utils::Init();
    SCENE_MGR.Init();
    InputMgr::Init();
}

void Framework::Do()
{
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        realDeltaTime = deltaTime = dt.asSeconds();
        deltaTime *= timeScale;
        realTime += realDeltaTime;
        time += deltaTime;

        InputMgr::Clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            InputMgr::UpdateEvent(event);
        }
        InputMgr::Update(deltaTime);
        
        if (InputMgr::GetKeyDown(sf::Keyboard::F10))
        {
            Variables::isDrawHitBox = !Variables::isDrawHitBox;
        }

        SCENE_MGR.Update(deltaTime);
        SCENE_MGR.LateUpdate(deltaTime);

        SCENE_MGR.FixedUpdate(deltaTime);

        window.clear();
        SCENE_MGR.Draw(window);
        window.display();
    }
}

void Framework::Release()
{
    SCENE_MGR.Release();
    FONT_MGR.Unload("fonts/DS-DIGI.ttf");
    DATATABLE_MGR.Release();
}
