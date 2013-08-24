#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "SoundGroup.h"
#include "MessageSystem.h"
#include "Level.h"

class Game
{
public:
    Game();
    ~Game();

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw();

    void Run();

private:
    sf::RenderWindow mWindow;
    const float mUpdateStep;

    enum State {
        SPLASH,
        INTRO,
        RUNNING,
        OVER
    } mState;

    sf::Sprite mSplashScreen;
    sf::Sprite mIntroScreen;
    sf::Sprite mGameOverScreen;

    sf::Music mBackgroundMusic;

    Level mLevel;
};
