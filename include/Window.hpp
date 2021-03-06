#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Utility.hpp"
#include "FileManager.hpp"
#include "MusicPlayer.hpp"
#include "PlayerSubWindow.hpp"
#include "FileSubWindow.hpp"
#include "FileManagerSubWindow.hpp"

#include <vector>
#include <string>

class Window
{
    public:
            Window(const std::string& programDir);
        
        void    draw() const;
        void    handleEvent(Event event);
        void    update();

    private:
        void    drawHelpScreen() const;
        void    buildHelpInfo();
        
    
    private:
        const Rectangle     mPlayerScreenBoundaries;
        const Rectangle     mFileScreenBoundaries;
        const Rectangle     mHelpScreenBoundaries;

        std::vector<SubWindow::ProgramEvent>    mProgramEvents;
        PlayerSubWindow         mPlayerSWindow;
        FileSubWindow           mFileSWindow;
        FileManagerSubWindow    mFileManSWindow;

        // FileManager         mFileManager;
        MusicPlayer             mMusicPlayer;
        // ProgramData         mProgData;

        const std::string&          mProgramDir;
        std::vector<std::string>    mHelpInfo;
        bool                        mHideHelpInfo;

};

#endif