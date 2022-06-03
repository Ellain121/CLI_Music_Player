#include "Application.hpp"

#include "ncurses.h"

#include <filesystem>


void start_ncurses()
{
    initscr();
    noecho();
    raw();
    start_color();
    curs_set(0);
    halfdelay(1.f);
    keypad(stdscr, true);
    init_pair(1, COLOR_GREEN, 0);
}

void end_ncurses()
{
    endwin();
}

int main(int argc, char* argv[])
{
    std::string programDir = std::filesystem::current_path().string();
    if (argc > 1)
    {
        programDir = argv[1];
        programDir = std::filesystem::weakly_canonical(programDir);
        if (!std::filesystem::exists(programDir))
        {
            std::string message = "Directory: ";
            message  += programDir;
            message  += " Does not exist";
            throw std::runtime_error(message.c_str());
        }
    }

    start_ncurses();

    Application app(programDir);
    app.run();

    end_ncurses();
}