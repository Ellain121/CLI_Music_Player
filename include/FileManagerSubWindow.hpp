#ifndef FILEMANAGERSUBWINDOW_HPP
#define FILEMANAGERSUBWINDOW_HPP

#include "SubWindow.hpp"
#include "FileManager.hpp"

#include <vector>

class FileManagerSubWindow : public SubWindow
{
    public:
            FileManagerSubWindow(Context context, const std::string& programDir,
                const Rectangle& bounds);
    
        void    draw() const;
        void    handleEvent(Event event);
        void    update();
    
    private:
        FileManager                 mFileManager;
};  

#endif