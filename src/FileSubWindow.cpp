#include "FileSubWindow.hpp"
#include "MusicPlayer.hpp"
#include "Utility.hpp"


#include "ncurses.h"

FileSubWindow::FileSubWindow(Context context, const std::string& programDir
    , const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mFileManager(programDir, bounds.getSizeY(), FileManager::Mode::MusicPlayer)
{
}


void FileSubWindow::drawHelpMenu() const
{
    auto& bounds = getBounds();

    auto visibleFiles = mFileManager.getVisibleFiles();

    int y = bounds.y1 + 1;
    int x = bounds.x1 + 2;

    mvprintw(y + 0, x, "(key_up) - select up");
    mvprintw(y + 1, x, "(key_down) - select down");
    mvprintw(y + 2, x, "(enter) - play selected");
    mvprintw(y + 3, x, "(s) - shuffle");
    mvprintw(y + 4, x, "(p) - play previous music");
    mvprintw(y + 5, x, "(n) - play next music");
    mvprintw(y + 6, x, "(f) - add to favorites");
    mvprintw(y + 7, x, "(h) - toggle help menu");
}

void FileSubWindow::draw() const
{
    drawRectangle();

    if (mHelpMenuMode)
    {
        drawHelpMenu();
    }
    else
    {
        auto& bounds = getBounds();

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
}

void FileSubWindow::update()
{
    SubWindow::update();

    auto& pEvents = *getContext().mProgEvents;

    // for (auto& pEvent : pEvents)
    for (int i = 0; i < pEvents.size(); ++i)
    {
        auto& pEvent = pEvents[i];
        if (pEvent.eventType == ProgramEventType::MusicFinished)
        {
            playNextMusic();    
            pEvents.erase(pEvents.begin() + i);
            i--;
        }
        if (pEvent.eventType == ProgramEventType::NewDirectory)
        {
            openNewDirectory(pEvent.info);
            pEvents.erase(pEvents.begin() + i);
            i--;
        }
        if (pEvent.eventType == ProgramEventType::NewDirectoryAll)
        {
            openNewDirectoryAll(pEvent.info);
            pEvents.erase(pEvents.begin() + i);
            i--;
        }
        if (pEvent.eventType == ProgramEventType::NewDirectoryAllAppend)
        {
            openNewDirectoryAllAppend(pEvent.info);
            pEvents.erase(pEvents.begin() + i);
            i--;
        }
    }
}

void FileSubWindow::openNewDirectory(const std::string& newDir)
{
    mFileManager.openDirectory(newDir);
}

void FileSubWindow::openNewDirectoryAll(const std::string& newDir)
{
    mFileManager.openDirectoryAll(newDir);
}

void FileSubWindow::openNewDirectoryAllAppend(const std::string& newDir)
{
    mFileManager.openDirectoryAllAppend(newDir);
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
    auto activeFilePathPtr = mFileManager.getActiveFilePath();

    musicPlayer.loadMusic(*activeFilePathPtr);
    musicPlayer.play();
}

void FileSubWindow::addToFavorites()
{
    auto favs = FileManager::getFavDirs();

    int answer = choose_option_window(favs);
    
    if (answer != -1)
    {
        auto activeFileNamePtr = mFileManager.getActiveFileName();
        auto activeFilePathPtr = mFileManager.getActiveFilePath();

        std::string old_path = *activeFilePathPtr;
        std::string new_path = FileManager::getFavDirLocation() + '/' + favs[answer] + '/' + *activeFileNamePtr;

        FileManager::createHardLink(old_path, new_path);
    }
        
}

void FileSubWindow::handleEvent(Event event)
{
    auto& musicPlayer = *getContext().musPlayer;
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
            musicPlayer.loadMusic(*mFileManager.getActiveFilePath());
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

        case 'f':
        {
            addToFavorites();
            auto& progEvents = *getContext().mProgEvents;
            progEvents.push_back(SubWindow::ProgramEventType::ReloadFileManagerFiles);

            break;
        }
        case 'h':
            mHelpMenuMode = !mHelpMenuMode;
            break;
    }
}