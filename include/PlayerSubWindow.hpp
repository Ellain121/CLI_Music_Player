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
        ProgressBar         mProgressBar;
};

#endif