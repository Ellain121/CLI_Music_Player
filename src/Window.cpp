#include "Window.hpp"

#include "ncurses.h"

#include <algorithm>

Window::Window(const std::string& programDir)
: mPlayerScreenBoundaries(1, COLS - 2, 1, 6)
, mFileScreenBoundaries(1, COLS / 2, 7, LINES - 2)
, mHelpScreenBoundaries(COLS / 2 + 2, COLS - 2, 7, LINES - 2)
, mPlayerSWindow(SubWindow::Context(mFileManager, mMusicPlayer, mProgData)
    , programDir, mPlayerScreenBoundaries)
, mFileSWindow(SubWindow::Context(mFileManager, mMusicPlayer, mProgData)
    , programDir, mFileScreenBoundaries)
, mFileManager(programDir, mFileScreenBoundaries.getSizeY())
, mProgramDir(programDir)
, mProgData()
, mHideHelpInfo(false)
{
    buildHelpInfo();
}

void Window::handleEvent(Event event)
{
    if (event == 'h')
    {
        mHideHelpInfo = !mHideHelpInfo;
    }
    if (event == 'i')
    {
        // TODO
    }
    if (event == 'f')
    {
        // TODO
    }

    mPlayerSWindow.handleEvent(event);
    mFileSWindow.handleEvent(event);
}

void Window::update()
{
    mPlayerSWindow.update();
    mFileSWindow.update();
}

void Window::draw() const
{
    clear();

    mPlayerSWindow.draw();
    mFileSWindow.draw();
    drawHelpScreen();

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