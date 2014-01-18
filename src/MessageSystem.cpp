#include "MessageSystem.h"


void MessageSystem::addMessage( const std::string& message, const sf::Vector2f& position, sf::Color color, float timeToLive /*= 2.f*/ )
{
    mMessages.push_back(Message(message, position, color, timeToLive));
}

void MessageSystem::update( float dt )
{
    for (size_t i = 0; i < mMessages.size();)
    {
        mMessages[i].timeToLive -= dt;
        if (mMessages[i].timeToLive <= 0.f)
        {
            mMessages[i] = mMessages[mMessages.size() - 1];
            mMessages.pop_back();
        }
        else
        {
            mMessages[i].string.Move(sf::Vector2f(0.f, -10.f) * dt);
            ++i;
        }
    }
}

void MessageSystem::draw( sf::RenderWindow& window )
{
    for (size_t i = 0; i < mMessages.size(); ++i)
        window.Draw(mMessages[i].string);
}

