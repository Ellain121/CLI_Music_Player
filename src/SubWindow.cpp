#include "SubWindow.hpp"

#include "ncurses.h"

SubWindow::ProgramEvent::ProgramEvent(ProgramEventType evType
    , std::string evInfo)
: eventType(evType)
, info(evInfo)
{
}

SubWindow::Context::Context(MusicPlayer& mPlayer, std::vector<ProgramEvent>& progEvents)
: musPlayer(&mPlayer)
, mProgEvents(&progEvents)
{
}

SubWindow::SubWindow(Context context, const std::string& programDir 
    , const Rectangle& bounds)
: mContext(context)
, mProgramDir(programDir)
, mBounds(bounds)
, mSubWindowSelected(false)
, mHelpMenuMode(false)
{
}

void SubWindow::drawRectangle() const
{
    int x1 = mBounds.x1;
    int x2 = mBounds.x2;
    int y1 = mBounds.y1;
    int y2 = mBounds.y2;

    if (mSubWindowSelected)
        attron(COLOR_PAIR(1));

    mvhline(y1, x1, 0, x2 - x1);
    mvhline(y2, x1, 0, x2 - x1);
    mvvline(y1, x1, 0, y2 - y1);
    mvvline(y1, x2, 0, y2 - y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);

    attroff(COLOR_PAIR(1));
}

void SubWindow::update()
{
    if (!iSSelected() && mHelpMenuMode)
    {
        mHelpMenuMode = false;
    }
}

void SubWindow::select()
{
    mSubWindowSelected = true;
}

void SubWindow::deselect()
{
    mSubWindowSelected = false;
}

bool SubWindow::iSSelected() const
{
    return mSubWindowSelected;
}

const Rectangle& SubWindow::getBounds() const
{
    return mBounds;
}

SubWindow::Context SubWindow::getContext() const
{
    return mContext;
}