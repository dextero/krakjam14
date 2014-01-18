#ifndef SRC_SOUND_GROUP_H
#define SRC_SOUND_GROUP_H

#include <SFML/Audio.hpp>
#include <vector>

class SoundGroup
{
public:
    SoundGroup(size_t maxSounds);
    void addSound(const std::string& sound, float volume = 100.f);

private:
    std::vector<sf::SoundBuffer> mSoundBuffers;
    std::vector<sf::Sound> mSounds;
};

#endif /* SRC_SOUND_GROUP_H */
