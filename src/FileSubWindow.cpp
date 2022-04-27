#include "FileSubWindow.hpp"
#include "MusicPlayer.hpp"


#include "ncurses.h"

FileSubWindow::FileSubWindow(Context context, const std::string& programDir
    , const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mFileManager(programDir, bounds.getSizeY())
{
}

void FileSubWindow::draw() const
{
    auto& bounds = getBounds();
    auto& programDir = getProgramDir();

    drawRectangle();
    auto visibleFiles = mFileManager.getVisibleFiles();

    int y = bounds.y1 + 1;
    int x = bounds.x1 + 2;

    std::size_t relativeSelectedIndx = mFileManager.getRelativeSelectedFileIndx();
    std::size_t playerFileRelativeIndx = mFileManager.getRelativeActiveFileIndx();

    for (size_t i = 0; i < visibleFiles.size(); ++i)
    {
        if (i == relativeSelectedIndx && iSSelected())
        {
            attron(A_REVERSE);
        }
        if (i == playerFileRelativeIndx)
        {
            attron(COLOR_PAIR(1));
        }
            print_in_boundaries(y, x, *visibleFiles[i], bounds.x2 - 4);
            attroff(A_REVERSE | COLOR_PAIR(1));
        y++;
    }
    size_t selectedFileIndx = mFileManager.getSelectedFileIndx();
    size_t filesAmount = mFileManager.getFilesAmount();
    int tempX = bounds.x2 - 10;
    int tempY = bounds.y2;
    mvprintw(tempY, tempX, "%d/%d", selectedFileIndx + 1, filesAmount);
}

void FileSubWindow::update()
{
    auto& pEvents = *getContext().mProgEvents;

    if (pEvents.size() > 0)
    {
        playNextMusic();
        pEvents.clear();
    }
}

void FileSubWindow::playNextMusic()
{
    if (!mFileManager.isAnyFileActivated()) return;

    mFileManager.activateDown();
    playActivedMusic();
}

void FileSubWindow::playPreviousMusic()
{
    if (!mFileManager.isAnyFileActivated()) return;

    mFileManager.activateUp();
    playActivedMusic();
}

void FileSubWindow::playActivedMusic()
{
    auto& musicPlayer = *getContext().musPlayer;
    auto& programDir = getProgramDir();
    auto activeFileNamePtr = mFileManager.getActiveFileName();

    musicPlayer.loadMusic(programDir + '/' + *activeFileNamePtr);
    musicPlayer.play();
}

void FileSubWindow::handleEvent(Event event)
{
    auto& musicPlayer = *getContext().musPlayer;
    auto& programDir = getProgramDir();
    switch (event)
    {
        case KEY_UP:
            mFileManager.selectUp();
            break;

        case KEY_DOWN:
            mFileManager.selectDown();
            break;

        case KEY_NORMAL_ENTER:
            mFileManager.activateSelectedFile();
            musicPlayer.loadMusic(programDir + '/' + *mFileManager.getActiveFileName());
            musicPlayer.play();
            break;
        
        case 's':
            mFileManager.shuffleFiles();
            break;

        case 'p':
            playPreviousMusic();
            break;

        case 'n':
            playNextMusic();
            break;
    }
}