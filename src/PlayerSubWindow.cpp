#include "PlayerSubWindow.hpp"
#include "SubWindow.hpp"

#include "ncurses.h"

#include <algorithm>

PlayerSubWindow::PlayerSubWindow(Context context, const std::string& programDir
    , const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mProgressBar('#', 50)
{

}

void PlayerSubWindow::draw() const
{
    auto& bounds = getBounds();
    auto& musicPlayer = *getContext().musPlayer;
    auto* activeFilePtr = getContext().progData->activeFile;

    draw_rectangle(bounds);
    int x = bounds.x1 + 5;
    int y = bounds.y1 + 1;

    if (activeFilePtr)
    {
        TimeInMinutes prgrs(musicPlayer.getProgressSeconds());
        TimeInMinutes dur(musicPlayer.getSongDuration());

        int diffX = std::min(bounds.x2 - 21, (int)activeFilePtr->size()) + 2;
        print_in_boundaries(y, x, *activeFilePtr, bounds.x2 - 21);
        mvprintw(y, x + diffX, "(%02d:%02d/%d:%02d)",
            prgrs.min, prgrs.sec, dur.min, dur.sec);
        std::string str = mProgressBar.getProgressBar(musicPlayer.getProgressPercent());
        mvprintw(y + 1, x, str.c_str());
        std::string str2 = std::to_string(int(musicPlayer.getVolume() * 100 + 0.5f));
        mvprintw(y + 2, x, "Volume: %s%%", str2.c_str());
    }
}

void PlayerSubWindow::update() 
{   
    auto& programDir = getProgramDir();
    auto& musicPlayer = *getContext().musPlayer;

    if (musicPlayer.isStreamDone())
    {
        playNextMusic();
    }
}

void PlayerSubWindow::handleEvent(Event event)
{
    auto& fileManager = *getContext().fileManager;
    auto& musicPlayer = *getContext().musPlayer;
    auto& programDir = getProgramDir();
    const std::string*& activeFilePtr = getContext().progData->activeFile;
    std::size_t& activeFileIndx = getContext().progData->activeFileIndx;

    switch (event)
    {
        case KEY_NORMAL_ENTER:
            activeFilePtr = fileManager.getSelectedFile();
            activeFileIndx = fileManager.getSelectedFileIndx();
            musicPlayer.loadMusic(programDir + '/' + *fileManager.getSelectedFile());
            musicPlayer.play();
            break;

        case 'p':
            playPreviousMusic();
            break;

        case 'n':
            playNextMusic();
            break;

        case '=':
            musicPlayer.volumeUp5Percent();
            break;
        
        case '-':
            musicPlayer.volumeDown5Percent();
            break;

        case KEY_RIGHT:
            musicPlayer.fastForward5Sec();
            break;

        case KEY_LEFT:
            musicPlayer.rewind5Sec();
            break;

        case ' ':
            musicPlayer.toggle();
            break;

        case 's':
            fileManager.shuffleFiles();
            break;
    }
}

void PlayerSubWindow::playNextMusic()
{
    selectNextMusic();
    playSelectedMusic();
}

void PlayerSubWindow::playPreviousMusic()
{
    selectPreviousMusic();
    playSelectedMusic();
}

void PlayerSubWindow::playSelectedMusic()
{
    auto& musicPlayer = *getContext().musPlayer;
    auto* activeFilePtr = getContext().progData->activeFile;
    auto& programDir = getProgramDir();

    musicPlayer.loadMusic(programDir + '/' + *activeFilePtr);
    musicPlayer.play();
}

void PlayerSubWindow::selectPreviousMusic()
{
    auto& fileManager = *getContext().fileManager;
    const std::string*& activeFilePtr = getContext().progData->activeFile;
    std::size_t& activeFileIndx = getContext().progData->activeFileIndx;

    if (activeFileIndx == 0)
    {
        activeFileIndx = fileManager.getFilesAmount() - 1;
    }
    else
    {
        activeFileIndx--;
    }
    activeFilePtr = fileManager.getFileByIndx(activeFileIndx);
}

void PlayerSubWindow::selectNextMusic()
{
    auto& fileManager = *getContext().fileManager;
    const std::string*& activeFilePtr = getContext().progData->activeFile;
    std::size_t& activeFileIndx = getContext().progData->activeFileIndx;

    activeFileIndx++;
    if (activeFileIndx >= fileManager.getFilesAmount())
    {
        activeFileIndx = 0;
    }
    activeFilePtr = fileManager.getFileByIndx(activeFileIndx);
}