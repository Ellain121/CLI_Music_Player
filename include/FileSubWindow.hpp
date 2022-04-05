#ifndef FILESUBWINDOW_HPP
#define FILESUBWINDOW_HPP

#include "SubWindow.hpp"

class FileSubWindow : public SubWindow
{
    public:
        FileSubWindow(Context context, const std::string& programDir
                    , const Rectangle& bounds);

        void    draw() const;
        void    handleEvent(Event event);
        void    update();
    
    private:
        
};

#endif
