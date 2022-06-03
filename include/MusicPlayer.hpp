#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include "bass.h"
#include <string>

class MusicPlayer
{
    public:
            MusicPlayer();

        void    loadMusic(const std::string& path);    
        void    play();
        void    stop();
        void    toggle();

        int     getProgressSeconds() const;
        int     getSongDuration() const;
        int     getProgressPercent() const;
        float   getVolume() const;

        void    fastForward5Sec();
        void    rewind5Sec();

        void    volumeUp5Percent();
        void    volumeDown5Percent();

        bool    isStreamDone();

        const std::string&  getCurrentMusicName() const;
        static bool         isFilePlayable(const std::string& path);
    
    private:
        float   getPreviousStreamVolume() const;
        void    setVolume(const float& volume);
        void    endStream();

    private:
        HSTREAM             mStream;
        bool                mFileCreated;
        bool                mMusicPlaying;
        std::string         mCurrentMusicName;
};

#endif