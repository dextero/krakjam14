#pragma once

#include "Player.h"

#include <memory>
#include <string>

#include <Box2D/Box2D.h>
#include <SFML/Graphics/RenderWindow.hpp>

class Level
{
public:
    Level();
    ~Level();

    bool load(const std::string& filename);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    inline bool isTimeOut() const;

    inline Player& getPlayer();

private:
    void debugDrawBodies(sf::RenderWindow& window);

    void addPlatform(const b2AABB& platform);

    std::unique_ptr<b2World> mWorld;
    Player mPlayer;

    float mTotalTime;
};

inline Player& Level::getPlayer()
{
    return mPlayer;
}

inline bool Level::isTimeOut() const
{
    return mTotalTime >= 10.f;
}
