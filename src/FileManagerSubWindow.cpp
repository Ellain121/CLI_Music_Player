#include "FileManagerSubWindow.hpp"

#include "ncurses.h"

#include <iostream>

FileManagerSubWindow::FileManagerSubWindow(Context context,
    const std::string& programDir, const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mFileManager(programDir, bounds.getSizeY(), FileManager::Mode::FilesManager)
, mInPlaylistDir(false)
, mDirBeforePlaylistDir("")
{
    if (!FileManager::isDirectoryExists(FAV_DIR))
    {
        throw std::logic_error(std::string("There is no program folder: ") + FAV_DIR + " not exist");
    }
}

void FileManagerSubWindow::drawHelpMenu() const
{
    auto& bounds = getBounds();

    int x = bounds.x1 + 2;
    int y = bounds.y1 + 1;

    mvprintw(y + 0, x, "(key_up) - select up");
    mvprintw(y + 1, x, "(key_down) - select down");
    mvprintw(y + 2, x, "(enter) - open selected directory");
    mvprintw(y + 3, x, "(o) - open directory in FileSubWi");
    mvprintw(y + 4, x + 2, "ndow(left window)");
    mvprintw(y + 5, x, "(f) - open playlists folder / go");
    mvprintw(y + 6, x + 2, "back where you have been before");
    mvprintw(y + 7, x, "(d) - delete playlist or file in");
    mvprintw(y + 8, x + 2, "playlist");
    mvprintw(y + 9, x, "(h) - toggle help menu");        
}

void FileManagerSubWindow::draw() const
{
    drawRectangle();

    if (mHelpMenuMode)
    {
        drawHelpMenu();
    }   
    else
    {
        auto& bounds = getBounds();
        const auto& currentDir = mFileManager.getCurrentDirectory();

        int x = bounds.x1 + 2;
        int y = bounds.y1 + 1;
        auto visibleFiles = mFileManager.getVisibleFiles();

        attron(COLOR_PAIR(1) | A_UNDERLINE);
        int tempX = (bounds.getSizeX()  - currentDir.size()) / 2;
        print_in_boundaries(y - 1, x, currentDir.c_str(), bounds.getSizeX() - 3);
        attroff(COLOR_PAIR(1) | A_UNDERLINE);

        std::size_t relativeSelectedIndx = mFileManager.getRelativeSelectedFileIndx();


        for (size_t i = 0; i < visibleFiles.size(); ++i)
        {
            if (i == relativeSelectedIndx && iSSelected())
            {
                attron(A_REVERSE);
            }

            print_in_boundaries(y, x, *visibleFiles[i], bounds.getSizeX() - 3);
            attroff(A_REVERSE | COLOR_PAIR(1));
            y++;
        }
        size_t selectedFileIndx = mFileManager.getSelectedFileIndx();
        size_t filesAmount = mFileManager.getFilesAmount();
        tempX = bounds.x2 - ((bounds.x2 - bounds.x1)/2);
        int tempY = bounds.y2;
        mvprintw(tempY, tempX, "%d/%d", selectedFileIndx + 1, filesAmount);
    }
}

void FileManagerSubWindow::handleEvent(Event event)
{
    auto& progEvents = *getContext().mProgEvents;

    switch (event)
    {
        case KEY_UP:
            mFileManager.selectUp();
            break;

        case KEY_DOWN:
            mFileManager.selectDown();
            break;

        case KEY_NORMAL_ENTER:
            mFileManager.openSelectedDirectory();
            break;
        
        case 'o':
        {
            const auto& currentDir = mFileManager.getCurrentDirectory();
            progEvents.push_back(ProgramEvent(ProgramEventType::NewDirectory
                , currentDir));
            break;
        }
        
        case 'f':
            // open app folder
            if (mInPlaylistDir)
            {
                mInPlaylistDir = false;
                mFileManager.openDirectory(mDirBeforePlaylistDir);
                mDirBeforePlaylistDir = "";
            }
            else
            {
                mDirBeforePlaylistDir = mFileManager.getCurrentDirectory();
                mFileManager.openDirectory(FAV_DIR);
                mInPlaylistDir = true;
            }
            break;
        
        case 'd':
        {
            if (!mInPlaylistDir) break;

            std::string curDir = mFileManager.getCurrentDirectory();
            std::string fileName = *mFileManager.getSelectedFileName();
            bool answer = yes_no_window("Do you want to delete " + fileName);
            if (answer == true)
            {
                FileManager::deleteAnyFile(curDir + '/' + fileName);
                mFileManager.reloadCurrentDirectory();
            }
            break;
        }
        
        case 'h':
            mHelpMenuMode = !mHelpMenuMode;
            break;


    }
}

void FileManagerSubWindow::update()
{
    SubWindow::update();

    auto& pEvents = *getContext().mProgEvents;

    for (int i = 0; i < pEvents.size(); ++i)
    {
        auto& pEvent = pEvents[i];
        if (pEvent.eventType == ProgramEventType::ReloadFileManagerFiles)
        {
            mFileManager.reloadCurrentDirectory();
            pEvents.erase(pEvents.begin() + i);
            i--;
        }
    }
}