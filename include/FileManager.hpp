#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <vector>
#include <string>

class FileManager
{
    public:
            FileManager(const std::string& directory, int screenLinesY);

        void            selectUp();
        void            selectDown();
        
        std::vector<const std::string*>     getVisibleFiles() const;
        std::size_t                         getSelectedFileIndx() const;
        std::size_t                         getRelativeSelectedFileIndx() const;
        const std::string*                  getSelectedFile() const;
        std::size_t                         getFilesAmount() const;
        std::size_t                         getFirstVisibleFileIndx() const;

        const std::string*                  getFileByIndx(std::size_t indx) const;

        void                                shuffleFiles();

    
    
    private:
        std::vector<std::string>    mFiles;
        std::size_t                 mFirstVisibleFileIndx;
        std::size_t                 mSelectedFileIndx;
        int                         mScreenSizeY;
};

#endif