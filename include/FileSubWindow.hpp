#ifndef FILESUBWINDOW_HPP
#define FILESUBWINDOW_HPP

#include "SubWindow.hpp"
#include "FileManager.hpp"

class FileSubWindow : public SubWindow
{
    public:
        FileSubWindow(Context context, const std::string& programDir
                    , const Rectangle& bounds);

        void    draw() const;
        void    drawHelpMenu() const;
        void    handleEvent(Event event);
        void    update();

    private:
        void        openNewDirectory(const std::string& newDir);
        void        openNewDirectoryAll(const std::string& newDir);
        void        openNewDirectoryAllAppend(const std::string& newDir);

        void        playNextMusic();
        void        playPreviousMusic();
        void        playActivedMusic();

        void        addToFavorites();
    
    private:
        FileManager     mFileManager;
        
};

#endif
