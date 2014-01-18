#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "SoundGroup.h"
#include "MessageSystem.h"
#include "Level.h"

static const sf::Vector2i WINDOW_SIZE(1024, 768);
static const sf::Vector2f WORLD_SIZE(WINDOW_SIZE.x / 3.f, WINDOW_SIZE.y / 3.f);

class Game
{
public:
    Game();
    ~Game();

    void init();

    void handleInput();
    void update(float dt);
    void draw();

    void run();

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
