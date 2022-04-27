#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

typedef int Event;

#define KEY_NORMAL_ENTER 10

// struct ProgramData
// {
//     ProgramData();
//     const std::string*  activeFile;
//     std::size_t         activeFileIndx;
// };  

struct Rectangle
{
    Rectangle();
    Rectangle(int x1, int x2, int y1, int y2);
    int getSizeX() const;
    int getSizeY() const;
    int x1;
    int x2;
    int y1;
    int y2;
};

struct TimeInMinutes
{
    TimeInMinutes(int seconds);
    int min;
    int sec;
};

struct ProgressBar
{
    ProgressBar(char prgrsChar, int size);
    std::string getProgressBar(int progressPercent) const;

    int     size;
    char    prgrsChar;
}; 

void draw_rectangle(const Rectangle& rec);
void print_in_boundaries(int y, int x, const std::string& fileName, int boundary);

bool yes_no_window(const std::string& text);

#endif