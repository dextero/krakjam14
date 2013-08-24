#ifndef SRC_ANIMATED_SPRITE_H
#define SRC_ANIMATED_SPRITE_H

#include <vector>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

class AnimatedSprite
{
public:
    AnimatedSprite();
    ~AnimatedSprite();

    void load(const std::string& filename, int numFrames, float fps);

    inline sf::Sprite& getSprite() { return mSprite; }
    sf::FloatRect getRect() const;
    void update(float dt);

private:private:
    sf::Sprite mSprite;
    std::vector<sf::Image*> mImages;

    float mUpdateTime;
    float mTimeAccumulator;
    int mCurrentFrame;
    int mNumFrames;
};

#endif /* SRC_ANIMATED_SPRITE_H */