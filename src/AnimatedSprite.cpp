#include "AnimatedSprite.h"

#include "ImageManager.h"

#include <sstream>
#include <cassert>

AnimatedSprite::AnimatedSprite()
{
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::load(const std::string& filename, int numFrames, float fps)
{
    mUpdateTime = 1.f / fps;
    mTimeAccumulator = 0.f;
    mCurrentFrame = 0;
    mNumFrames = numFrames;
    mImages.clear();

    std::stringstream ss;
    size_t dotAt = filename.rfind('.');

    assert(dotAt != std::string::npos);

    const std::string filenameBase = filename.substr(0, dotAt);
    const std::string filenameExtension = filename.substr(dotAt);

    for (int i = 0; i < numFrames; ++i)
    {
        ss.str("");
        ss << filenameBase << i << filenameExtension;

        sf::Image* img = &gImgMgr.get(ss.str());
        mImages.push_back(img);
    }

    mSprite.SetImage(*mImages[0]);
    mSprite.SetCenter(mSprite.GetSize() * .5f);
}

void AnimatedSprite::update( float dt )
{
    assert(mImages.size() > 0);

    mTimeAccumulator += dt;
    if (mTimeAccumulator > mUpdateTime)
    {
        mTimeAccumulator -= mUpdateTime;
        mCurrentFrame = (++mCurrentFrame) % mNumFrames;
        mSprite.SetImage(*mImages[mCurrentFrame]);
    }
}

sf::FloatRect AnimatedSprite::getRect() const
{
    sf::Vector2f halfSize = mSprite.GetSize() * 0.5f;
    return sf::FloatRect(mSprite.GetPosition().x - halfSize.x,
                         mSprite.GetPosition().y - halfSize.y,
                         mSprite.GetPosition().x + halfSize.x,
                         mSprite.GetPosition().y + halfSize.y);
}
