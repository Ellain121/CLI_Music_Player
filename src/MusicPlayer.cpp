#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
: mFileCreated(false)
, mMusicPlaying(false)
{
    BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, 0);
}

bool MusicPlayer::isStreamDone()
{
    if (!BASS_ChannelIsActive(mStream) && mMusicPlaying)
    {
        mMusicPlaying  = false;
        return true;
    }
    return false;
}

void MusicPlayer::loadMusic(const std::string& path)
{
    endStream();
    
    mFileCreated = true;
    mStream = BASS_StreamCreateFile(0, path.c_str(), 0, 0, 0);
}

void MusicPlayer::play()
{
    BASS_ChannelPlay(mStream, false);
    mMusicPlaying = true;
}

void MusicPlayer::stop()
{
    BASS_ChannelStop(mStream);
    mMusicPlaying = false;
}

void MusicPlayer::toggle()
{
    if (mMusicPlaying)
    {
        stop();
    }
    else
    {
        play();
    }
}

int MusicPlayer::getProgressSeconds() const
{
    double prog = BASS_ChannelBytes2Seconds(mStream, 
        BASS_ChannelGetPosition(mStream, BASS_POS_BYTE));
    return (int)prog;
}

int MusicPlayer::getSongDuration() const
{
    double duration = BASS_ChannelBytes2Seconds(mStream, 
        BASS_ChannelGetLength(mStream, BASS_POS_BYTE));
    
    return (int)duration;
}

int MusicPlayer::getProgressPercent() const
{
    return getProgressSeconds() * 100 / getSongDuration();
}

void MusicPlayer::fastForward5Sec()
{
    if (!mFileCreated) return;

    double newPos = BASS_ChannelBytes2Seconds(mStream, 
        BASS_ChannelGetPosition(mStream, BASS_POS_BYTE)) + 5.0;
    
    if (newPos >= getSongDuration())
    {
        endStream();
    }

    BASS_ChannelSetPosition(mStream, 
        BASS_ChannelSeconds2Bytes(mStream, newPos), BASS_POS_BYTE);
}

void MusicPlayer::rewind5Sec()
{
    if (!mFileCreated) return;
    
    double newPos = BASS_ChannelBytes2Seconds(mStream, 
        BASS_ChannelGetPosition(mStream, BASS_POS_BYTE)) - 5.0;
    
    if (newPos < 0.0)
    {
        newPos = 0.0;
    }

    BASS_ChannelSetPosition(mStream, 
        BASS_ChannelSeconds2Bytes(mStream, newPos), BASS_POS_BYTE);
}

void MusicPlayer::setVolume(const float& volume)
{
    BASS_ChannelSetAttribute(mStream, BASS_ATTRIB_VOL, volume);
}

float MusicPlayer::getVolume() const
{
    float volume;
    BASS_ChannelGetAttribute(mStream, BASS_ATTRIB_VOL, &volume);
    return volume;
}

void MusicPlayer::volumeUp5Percent()
{
    float volume = getVolume();
    volume += 0.05f;
    setVolume(volume);
}

void MusicPlayer::volumeDown5Percent()
{
    float volume = getVolume();
    volume -= 0.05f;
    setVolume(volume);
}

void MusicPlayer::endStream()
{
    if (mFileCreated == true)
    {
        BASS_StreamFree(mStream);
        mFileCreated = false;
    }
}