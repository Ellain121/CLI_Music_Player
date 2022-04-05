#include "FileSubWindow.hpp"
#include "FileManager.hpp"
#include "MusicPlayer.hpp"


#include "ncurses.h"

FileSubWindow::FileSubWindow(Context context, const std::string& programDir
    , const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
{
}

void FileSubWindow::draw() const
{
    auto& bounds = getBounds();
    auto& fileManager = *getContext().fileManager;
    auto& programDir = getProgramDir();
    auto& activeFileIndx = getContext().progData->activeFileIndx;

    draw_rectangle(bounds);
    auto visibleFiles = fileManager.getVisibleFiles();

    int y = bounds.y1 + 1;
    int x = bounds.x1 + 2;

    attron(COLOR_PAIR(1) | A_UNDERLINE);
    int tempX = (bounds.getSizeX()  - programDir.size()) / 2;
    mvprintw(y - 1, tempX, programDir.c_str());
    attroff(COLOR_PAIR(1) | A_UNDERLINE);

    std::size_t relativeSelectedIndx = fileManager.getRelativeSelectedFileIndx();
    std::size_t playerFileRelativeIndx = activeFileIndx - fileManager.getFirstVisibleFileIndx();

    for (size_t i = 0; i < visibleFiles.size(); ++i)
    {
        if (i == relativeSelectedIndx)
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
    size_t selectedFileIndx = fileManager.getSelectedFileIndx();
    size_t filesAmount = fileManager.getFilesAmount();
    tempX = bounds.x2 - 10;
    int tempY = bounds.y2;
    mvprintw(tempY, tempX, "%d/%d", relativeSelectedIndx + 1, filesAmount);
}

void FileSubWindow::update()
{

}

void FileSubWindow::handleEvent(Event event)
{
    auto& fileManager = *getContext().fileManager;

    if (event == KEY_UP)
    {
        fileManager.selectUp();
    }
    if (event == KEY_DOWN)
    {
        fileManager.selectDown();
    }
}