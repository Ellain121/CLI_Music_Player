#ifndef PLAYERSUBWINDOW_HPP
#define PLAYERSUBWINDOW_HPP

#include "SubWindow.hpp"
#include "MusicPlayer.hpp"
#include "FileManager.hpp"

#include <string>

class PlayerSubWindow : public SubWindow
{
    public:
        PlayerSubWindow(Context context, const std::string& programDir
                    , const Rectangle& bounds);

    void    draw() const;
    void    handleEvent(Event event);
    void    update();

    private:
        void    playNextMusic();
        void    playPreviousMusic();

        void    selectNextMusic();
        void    selectPreviousMusic();

        void    playSelectedMusic();
    
    private:
        ProgressBar         mProgressBar;
};

#endif