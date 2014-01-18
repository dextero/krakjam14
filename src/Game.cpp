#include "Game.h"

#include "utils/Logger.h"
#include "ImageManager.h"

#include <SFML/Window/Input.hpp>

#include <sstream>

Game::Game():
    mUpdateStep(1.f / 30.f)
{
    mSplashScreen.SetImage(gImgMgr.get("data/splash.png"));
    mIntroScreen.SetImage(gImgMgr.get("data/intro.png"));
    mGameOverScreen.SetImage(gImgMgr.get("data/gameover.png"));

//    mBackgroundMusic.Stop();
//    mBackgroundMusic.OpenFromFile("data/settlers4-mayan.ogg");
//    mBackgroundMusic.SetVolume(100.f);
//    mBackgroundMusic.SetLoop(true);
//    mBackgroundMusic.Play();
}

Game::~Game()
{

}

void Game::init()
{
    Log.log(Logger::MESSAGE, "initializing...\n");

    mState = RUNNING;

//    mBackgroundMusic.Stop();
//    mBackgroundMusic.OpenFromFile("data/settlers4-mayan.ogg");
//    mBackgroundMusic.SetVolume(100.f);
//    mBackgroundMusic.SetLoop(true);
//    mBackgroundMusic.Play();

    Log.log(Logger::MESSAGE, "game initialized\n");
}

void Game::handleInput()
{
    sf::Event ev;

    while (mWindow.GetEvent(ev))
    {
        switch (mState)
        {
        case SPLASH:
            if (ev.Type == sf::Event::KeyPressed ||
                ev.Type == sf::Event::MouseButtonPressed)
                mState = INTRO;
            break;
        case INTRO:
            if (ev.Type == sf::Event::KeyPressed ||
                ev.Type == sf::Event::MouseButtonPressed)
                init();
            break;
        case RUNNING:
            switch (ev.Type)
            {
            case sf::Event::MouseMoved:
                break;
            case sf::Event::MouseButtonPressed:
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::KeyPressed:
                switch (ev.Key.Code)
                {
                case sf::Key::F1:
                    mLevel.load("");
                    break;
                case sf::Key::Space:
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                switch (ev.Key.Code)
                {
                default:
                    break;
                }
            default:
                break;
            }
            break;
        case OVER:
            if (ev.Type == sf::Event::KeyPressed)
                init();
            break;
        }

        // global
        switch (ev.Type)
        {
        case sf::Event::KeyPressed:
            switch (ev.Key.Code)
            {
            case sf::Key::Escape:
                mWindow.Close();
                break;
            default:
                break;
            }
            break;
        case sf::Event::Closed:
            mWindow.Close();
            break;
        default:
            break;
        }
    }
}

void Game::update( float dt )
{
    switch (mState)
    {
    case SPLASH:
        mState = RUNNING;
        break;
    case INTRO:
        mState = RUNNING;
        break;
    case RUNNING:
        mLevel.update(dt);
        break;
    case OVER:
        break;
    }
}

void Game::draw()
{
    mWindow.Clear(sf::Color::Black);

    switch (mState)
    {
    case SPLASH:
        mWindow.Draw(mSplashScreen);
        break;
    case INTRO:
        mWindow.Draw(mIntroScreen);
        break;
    case RUNNING:
        mLevel.draw(mWindow);
        break;
    case OVER:
        mWindow.Draw(mGameOverScreen);
        break;
    }

    mWindow.Display();
}

void Game::run()
{
    Log.log(Logger::MESSAGE, "game started\n");

    mWindow.Create(
        sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
        "$GAME_TITLE",
        sf::Style::Close);

    mWindow.SetView(sf::View(sf::FloatRect(
        0.f, WORLD_SIZE.y, WORLD_SIZE.x, 0.f)));

    mWindow.ShowMouseCursor(false);

    mState = SPLASH;

    static sf::Clock clock;
    static float timeAccumulator = 0.f;

    while (mWindow.IsOpened())
    {
        handleInput();

        timeAccumulator += clock.GetElapsedTime();
        clock.Reset();

        while (timeAccumulator > mUpdateStep)
        {
            update(mUpdateStep);
            timeAccumulator -= mUpdateStep;
        }

        draw();
    }

    Log.log(Logger::MESSAGE, "game closed\n");
}
