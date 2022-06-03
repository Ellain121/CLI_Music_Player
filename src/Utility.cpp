#include "Utility.hpp"

#include "ncurses.h"
#include "FileManager.hpp"

#include <algorithm>

Rectangle::Rectangle()
: x1(0), x2(0), y1(0), y2(0)
{
}

Rectangle::Rectangle(int x1, int x2, int y1, int y2)
: x1(x1), x2(x2), y1(y1), y2(y2)
{
}

int Rectangle::getSizeX() const
{
    return x2 - x1;
}

int Rectangle::getSizeY() const
{
    return y2 - y1;
}

TimeInMinutes::TimeInMinutes(int seconds)
{
    min = seconds / 60;
    sec = seconds % 60;
}

ProgressBar::ProgressBar(char prgrsChar, int size)
: prgrsChar(prgrsChar)
, size(size)
{
}

std::string ProgressBar::getProgressBar(int progressPercent) const
{
    std::string prgrsBar(size + 2, ' ');
    prgrsBar[0] = '[';
    prgrsBar[prgrsBar.size() - 1] = ']';

    int filledCells = size * progressPercent / 100;
    for (int i = 0; i < filledCells; ++i)
    {
        prgrsBar[i + 1] = prgrsChar;
    }
    return prgrsBar;
}

void draw_rectangle(const Rectangle& rec)
{
    int x1 = rec.x1;
    int x2 = rec.x2;
    int y1 = rec.y1;
    int y2 = rec.y2;
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

void print_in_boundaries(int y, int x, const std::string& fileName, int boundary)
{
    if (fileName.size() > boundary)
    {
        for (int i = 0; i < boundary - 2; ++i)  
        {
            mvprintw(y, x + i, "%c", fileName[i]);
        }
        mvprintw(y, x + boundary - 2, "..");
    }
    else
    {
        mvprintw(y, x, fileName.c_str());
    }
}

bool yes_no_window(const std::string& text)
{
    clear();
    int sizeX = COLS / 2; 
    int sizeY = LINES / 2;
    int offX = COLS / 4;
    int offY = LINES / 4;
    draw_rectangle(Rectangle(offX, offX + sizeX, offY, offY + sizeY));

    int x = offX + (sizeX - text.size()) / 2;
    int y = offY + 3;

    bool optionNoSelected = true;

    mvprintw(y, x, text.c_str());
    attron(A_REVERSE);
    mvprintw(y + 3, x + 3, "No");
    attroff(A_REVERSE);
    mvprintw(y + 3, x + 10, "Yes");

    int ch = getch();
    while (ch != KEY_NORMAL_ENTER)
    {
        int ch = getch();

        if (ch == KEY_RIGHT) optionNoSelected = false;
        if (ch == KEY_LEFT) optionNoSelected = true;
        if (ch == KEY_NORMAL_ENTER)
        {
            return !optionNoSelected;
        }

        mvprintw(y, x, text.c_str());
        if (optionNoSelected)
        {
            attron(A_REVERSE);
            mvprintw(y + 3, x + 3, "No");
            attroff(A_REVERSE);
            mvprintw(y + 3, x + 10, "Yes");
        }
        else
        {
            mvprintw(y + 3, x + 3, "No");
            attron(A_REVERSE);
            mvprintw(y + 3, x + 10, "Yes");
            attroff(A_REVERSE);
        }

        refresh();
    }

    return true;
}

std::size_t choose_option_window(std::vector<std::string>& options)
{
    options.push_back("create new playlist");
    clear();
    int sizeX = COLS / 2; 
    int sizeY = std::min((int)options.size() + 3, COLS - 2);
    int offX = COLS / 4;
    int offY = LINES / 4;

    draw_rectangle(Rectangle(offX, offX + sizeX, offY, offY + sizeY));

    int selectedIndx = 0;

    std::function<void()> drawOptions = [&]{
        std::string msg = "Choose option: ";
        mvprintw(offY, offX + (sizeX - msg.size()) / 2, msg.c_str());
        for (int i = 0; i < options.size(); ++i)
        {
            const std::string& option = options[i];

            int x = offX + 3;
            int y = offY + 2 + i;

            if (i == selectedIndx)
            {
                attron(A_REVERSE);
                mvprintw(y, x, option.c_str());
                attroff(A_REVERSE);
            }
            else
            {
                mvprintw(y, x, option.c_str());
            }
        };
    };
    drawOptions();

    int ch;
    while (true)
    {
        int ch = getch();

        if (ch == KEY_DOWN) selectedIndx = selectedIndx + 1 < options.size() ? selectedIndx + 1 : 0;
        if (ch == KEY_UP) selectedIndx = selectedIndx - 1 >= 0 ? selectedIndx - 1 : options.size() - 1;
        if (ch == KEY_NORMAL_ENTER)
        {
            if (options[selectedIndx] == "create new playlist")
            {
                auto res = create_new_playlist_window();
                if (res.second == true)
                {
                    options[selectedIndx] = res.first;
                    return selectedIndx;
                }
                else
                {
                    draw_rectangle(Rectangle(offX, offX + sizeX, offY, offY + sizeY));
                }
            }
            else
            {
                return selectedIndx;
            }
        }

        drawOptions();

        refresh();
    }

    return 0; 
}

// return value pair (1) - playlist name;  (2) - if successfull, if not, then don't use (1)
std::pair<std::string, bool> create_new_playlist_window()
{
    int sizeX = COLS / 2; 
    int sizeY = LINES / 2;
    int offX = COLS / 4;
    int offY = LINES / 4;
    curs_set(1);

    std::string text = "";
    auto draw_func = [&]{
        clear();
        draw_rectangle(Rectangle(offX, offX + sizeX, offY, offY + sizeY));
        std::string msg = "write playlist name";
        mvprintw(offY, offX + sizeX/2 - msg.size() / 2, msg.c_str());

        mvprintw(offY + 3, offX + 2, ": ");
        mvprintw(offY + 3, offX + 4, text.c_str());
        refresh();
    };

    int ch;
    while (true)
    {
        draw_func();
        int ch = getch();
        if (ch == KEY_NORMAL_ENTER)
        {
            break;
        }
        if (ch == KEY_BACKSPACE && !text.empty())
        {
            text.pop_back();
        }

        if (!isprint(ch)) continue;

        text += ch;

    }

    curs_set(0);
    clear();
    if (!text.empty())
    {
        std::string folderPath = std::string(FAV_DIR) + text;
        FileManager::createFolder(folderPath);
        
        return std::make_pair(text, true);
    }
    else
    {
        return std::make_pair(text, false);
    }
}