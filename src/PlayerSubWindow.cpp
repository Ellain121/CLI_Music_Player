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

void PlayerSubWindow::drawHelpMenu() const
{
    auto& bounds = getBounds();
    auto& musicPlayer = *getContext().musPlayer;

    int x = bounds.x1 + 2;
    int y = bounds.y1 + 1;

    mvprintw(y + 0, x, "(=) - up volume by 5 %");
    mvprintw(y + 1, x, "(-) - down volume by 5 %");
    mvprintw(y + 2, x, "(key_right) - fast forward 5 sec");

    mvprintw(y + 0, x + 40, "| (key_left) - rewind 5 sec");
    mvprintw(y + 1, x + 40, "| ( ) - pause");
    mvprintw(y + 2, x + 40, "| (h) - toggle help menu");
}

void PlayerSubWindow::draw() const
{
    drawRectangle();

    if (mHelpMenuMode)
    {
        drawHelpMenu();
    }
    else
    {
        auto& bounds = getBounds();
        auto& musicPlayer = *getContext().musPlayer;

        auto& musicName = musicPlayer.getCurrentMusicName();
        int x = bounds.x1 + 5;
        int y = bounds.y1 + 1;

        if (!musicName.empty())
        {
            TimeInMinutes prgrs(musicPlayer.getProgressSeconds());
            TimeInMinutes dur(musicPlayer.getSongDuration());

            int diffX = std::min(bounds.x2 - 21, (int)musicName.size()) + 2;
            print_in_boundaries(y, x, musicName, bounds.x2 - 21);
            mvprintw(y, x + diffX, "(%02d:%02d/%d:%02d)",
                prgrs.min, prgrs.sec, dur.min, dur.sec);
            std::string str = mProgressBar.getProgressBar(musicPlayer.getProgressPercent());
            mvprintw(y + 1, x, str.c_str());
            std::string str2 = std::to_string(int(musicPlayer.getVolume() * 100 + 0.5f));
            mvprintw(y + 2, x, "Volume: %s%%", str2.c_str());
        }
    }
}

void PlayerSubWindow::update() 
{   
    SubWindow::update();

    auto& musicPlayer = *getContext().musPlayer;
    auto& progEvents = *getContext().mProgEvents;

    if (musicPlayer.isStreamDone())
    {
        // playNextMusic();
        progEvents.push_back(ProgramEvent(ProgramEventType::MusicFinished));
    }
}

void PlayerSubWindow::handleEvent(Event event)
{
    auto& musicPlayer = *getContext().musPlayer;
    switch (event)
    {
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
        
        case 'h':
            if (iSSelected())
            {
                mHelpMenuMode = !mHelpMenuMode;
            }
            break;
    }
}