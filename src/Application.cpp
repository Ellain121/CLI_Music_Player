#include "Application.hpp"
#include "Utility.hpp"

#include "ncurses.h"

#include <iostream>

Application::Application(const std::string& programDir)
: mWindow(programDir)
, mProgramDir(programDir)
{
}

void Application::run()
{
    while(true)
    {
        Event event = getch();
        if (event == 'q')
        {
            bool answer = yes_no_window("Close application?");
            if (answer)
                break;
        }
        mWindow.handleEvent(event);
        mWindow.update();
        mWindow.draw();
    }
}