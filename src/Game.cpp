#include "Game.h"

#include "utils/Logger.h"
#include "ImageManager.h"

#include <SFML/Window/Input.hpp>

#include <sstream>

Game::Game():
    mUpdateStep(1.f / 30.f)
{
    mSplashScreen.SetImage(gImgMgr.Get("data/splash.png"));
    mIntroScreen.SetImage(gImgMgr.Get("data/intro.png"));
    mGameOverScreen.SetImage(gImgMgr.Get("data/gameover.png"));

//    mBackgroundMusic.Stop();
//    mBackgroundMusic.OpenFromFile("data/settlers4-mayan.ogg");
//    mBackgroundMusic.SetVolume(100.f);
//    mBackgroundMusic.SetLoop(true);
//    mBackgroundMusic.Play();
}

Game::~Game()
{

}

void Game::Init()
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

void Game::HandleInput()
{
    sf::Event ev;

    mLevel.GetPlayer().Jump(mWindow.GetInput().IsKeyDown(sf::Key::W));
    mLevel.GetPlayer().Move(
        mWindow.GetInput().IsKeyDown(sf::Key::A) ? MoveLeft :
            mWindow.GetInput().IsKeyDown(sf::Key::D) ? MoveRight : MoveStop);

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
                Init();
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
                    mLevel.Load("");
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
                Init();
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

void Game::Update( float dt )
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
        mLevel.Update(dt);
        break;
    case OVER:
        break;
    }
}

void Game::Draw()
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
        mLevel.Draw(mWindow);
        break;
    case OVER:
        mWindow.Draw(mGameOverScreen);
        break;
    }

    mWindow.Display();
}

void Game::Run()
{
    Log.log(Logger::MESSAGE, "game started\n");

    static const sf::Vector2i WINDOW_SIZE(1024, 768);

    mWindow.Create(
        sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
        "$GAME_TITLE",
        sf::Style::Close);

    mWindow.SetView(sf::View(sf::FloatRect(
        0.f, (float)WINDOW_SIZE.y,
        (float)WINDOW_SIZE.x, 0.f)));

    mWindow.ShowMouseCursor(false);

    mState = SPLASH;

    static sf::Clock clock;
    static float timeAccumulator = 0.f;

    while (mWindow.IsOpened())
    {
        HandleInput();

        timeAccumulator += clock.GetElapsedTime();
        clock.Reset();

        while (timeAccumulator > mUpdateStep) 
        {
            Update(mUpdateStep);
            timeAccumulator -= mUpdateStep;
        }

        Draw();
    }

    Log.log(Logger::MESSAGE, "game closed\n");
}
