#ifndef SRC_MESSAGESYSTEM_H
#define SRC_MESSAGESYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/String.hpp>
#include <string>

class MessageSystem
{
public:
    void addMessage(const std::string& message, const sf::Vector2f& position, sf::Color color = sf::Color::Green, float timeToLive = 2.f);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    struct Message {
        sf::String string;
        float timeToLive;

        Message(const std::string& msg, const sf::Vector2f& pos, sf::Color color, float ttl):
            string(msg),
            timeToLive(ttl)
        {
            string.SetSize(24.f);
            string.SetColor(color);
            string.SetPosition(pos);
        }
    };

    std::vector<Message> mMessages;
};

#endif /* SRC_MESSAGESYSTEM_H */