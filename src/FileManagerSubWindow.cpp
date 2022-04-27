#include "FileManagerSubWindow.hpp"

#include "ncurses.h"

FileManagerSubWindow::FileManagerSubWindow(Context context,
    const std::string& programDir, const Rectangle& bounds)
: SubWindow(context, programDir, bounds)
, mFileManager(programDir, bounds.getSizeY(), FileManager::Mode::FilesManager)
{

}

void FileManagerSubWindow::draw() const
{
    auto& bounds = getBounds();
    const auto& currentDir = mFileManager.getCurrentDirectory();

    drawRectangle();

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
            const auto& currentDir = mFileManager.getCurrentDirectory();
            progEvents.push_back(ProgramEvent(ProgramEventType::NewDirectory
                , currentDir));
            break;
    }
}

void FileManagerSubWindow::update()
{

}