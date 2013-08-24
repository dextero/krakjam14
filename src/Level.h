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

    bool Load(const std::string& filename);

    void Update(float dt);
    void Draw(sf::RenderWindow& window);

    inline Player& GetPlayer();

private:
    void DebugDrawBodies(sf::RenderWindow& window);

    void AddPlatform(const b2AABB& platform);

    std::unique_ptr<b2World> mWorld;
    Player mPlayer;
};

inline Player& Level::GetPlayer()
{
    return mPlayer;
}
