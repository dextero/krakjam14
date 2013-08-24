#include "SoundGroup.h"

#include "utils/Utils.h"

SoundGroup::SoundGroup( size_t maxSounds )
{
    mSounds.resize(maxSounds);
    mSoundBuffers.resize(maxSounds);
}

void SoundGroup::AddSound( const std::string& sound, float volume )
{
    for (size_t i = 0; i < mSounds.size(); ++i)
    {
        if (mSounds[i].GetStatus() == sf::Sound::Stopped)
        {
            mSoundBuffers[i].LoadFromFile(sound);
            mSounds[i].SetBuffer(mSoundBuffers[i]);
            mSounds[i].SetVolume(volume);
            mSounds[i].Play();
        }
    }
}