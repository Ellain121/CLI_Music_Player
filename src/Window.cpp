#include "Window.hpp"

#include "ncurses.h"

#include <algorithm>

Window::Window(const std::string& programDir)
: mPlayerScreenBoundaries(1, COLS - 2, 1, 6)
, mFileScreenBoundaries(1, COLS / 2, 7, LINES - 2)
, mHelpScreenBoundaries(COLS / 2 + 2, COLS - 2, 7, LINES - 2)
, mProgramEvents()
, mPlayerSWindow(SubWindow::Context(mMusicPlayer, mProgramEvents)
    , programDir, mPlayerScreenBoundaries)
, mFileSWindow(SubWindow::Context(mMusicPlayer, mProgramEvents)
    , programDir, mFileScreenBoundaries)
, mFileManSWindow(SubWindow::Context(mMusicPlayer, mProgramEvents)
    , programDir, mHelpScreenBoundaries)
// , mFileManager(programDir, mFileScreenBoundaries.getSizeY())
, mProgramDir(programDir)
// , mProgData()
, mHideHelpInfo(false)
{
    buildHelpInfo();
    mFileSWindow.select();
}

void Window::handleEvent(Event event)
{
    switch (event)
    {
        case '1':
            mPlayerSWindow.select();
            mFileSWindow.deselect();
            mFileManSWindow.deselect();
        break;

        case '2':
            mPlayerSWindow.deselect();
            mFileSWindow.select();
            mFileManSWindow.deselect();
        break;

        case '3':
            mPlayerSWindow.deselect();
            mFileSWindow.deselect();
            mFileManSWindow.select();

        case 'h':
            mHideHelpInfo = !mHideHelpInfo;
        break;

        case 'i':
            // TODO
        break;

        case 'f':
            // TODO
        break;
    }

    mPlayerSWindow.handleEvent(event);

    if (mFileSWindow.iSSelected())
        mFileSWindow.handleEvent(event);
        
    if (mFileManSWindow.iSSelected())    
        mFileManSWindow.handleEvent(event);        

}

void Window::update()
{
    mPlayerSWindow.update();
    mFileSWindow.update();
    mFileManSWindow.update();
}

void Window::draw() const
{
    clear();

    mPlayerSWindow.draw();
    mFileSWindow.draw();
    mFileManSWindow.draw();
    // drawHelpScreen();

    refresh();
}

void Window::drawHelpScreen() const
{
    if (mHideHelpInfo) return;

    int x = mHelpScreenBoundaries.x1;
    int y = mHelpScreenBoundaries.y1 + 1;

    for (int i = 0; i < mHelpInfo.size(); ++i)
    {
        mvprintw(y + i, x, mHelpInfo[i].c_str());
    }
}

void Window::buildHelpInfo()
{
    mHelpInfo = {
        {"[key right] - Fast forward 5 seconds"},
        {"[key left] - Rewind 5 seconds"},
        {"[key up/down] - Select track"},
        {"[enter] - Play selected track"},
        {"[space] - Pause/Continue"},
        {"[n] - Next track"},
        {"[p] - Previous track"},
        {"[=] - Volume up"},
        {"[-] - Volume down"},
        {"[s] - Shuffle"},
        {"[f] - Find track by name"},
        {"[i] - Find track by number"},
        {"[h] - Show/Hide help menu"},
        {"[q] - Quit"},
    };
}