#include "FileManagerSubWindow.hpp"

#include "ncurses.h"

FileManagerSubWindow::FileManagerSubWindow(Context context,
    const std::string& programDir, const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mFileManager(programDir, bounds.getSizeY())
, mCurrentDir(programDir)
{

}

void FileManagerSubWindow::draw() const
{
    auto& bounds = getBounds();

    drawRectangle();

    int x = bounds.x1 + 2;
    int y = bounds.y1 + 1;
    auto visibleFiles = mFileManager.getVisibleFiles();

    attron(COLOR_PAIR(1) | A_UNDERLINE);
    int tempX = (bounds.getSizeX()  - mCurrentDir.size()) / 2;
    print_in_boundaries(y - 1, x, mCurrentDir.c_str(), bounds.getSizeX() - 3);
    attroff(COLOR_PAIR(1) | A_UNDERLINE);

    std::size_t relativeSelectedIndx = mFileManager.getRelativeSelectedFileIndx();
    std::size_t playerFileRelativeIndx = mFileManager.getRelativeActiveFileIndx();


    for (size_t i = 0; i < visibleFiles.size(); ++i)
    {
        print_in_boundaries(y, x, *visibleFiles[i], bounds.getSizeX() - 3);
        y++;
    }
    size_t selectedFileIndx = mFileManager.getSelectedFileIndx();
    size_t filesAmount = mFileManager.getFilesAmount();
    tempX = bounds.x2 - ((bounds.x2 - bounds.x1)/2);
    int tempY = bounds.y2;
    mvprintw(tempY, tempX, "%d/%d", selectedFileIndx + 1, filesAmount);
}

void FileManagerSubWindow::handleEvent(Event event)
{

}

void FileManagerSubWindow::update()
{

}