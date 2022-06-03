#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <vector>
#include <string>

/**
 *  FileManager
 * 
 *  Can get info about files in current directory
 *  Store all indices
 *  Have limit visible area, in which currently selected file located
 *  Visible area moves throughout all files, when selected file outside of
 *      visible area.
 *  Active file - just feature for player, cause when song is done, next one
 *      should be playing, and user should see current playing song
 * 
 *  active files
 * 
 *  selected files
 * 
 * 
 */

class FileManager
{
    public:
        enum Mode
        {
            None,
            MusicPlayer,
            FilesManager,
        };

    public:
            FileManager(const std::string& directory, int screenLinesY,
                Mode mode);

        void            selectUp();
        void            selectDown();

        void            activateUp();
        void            activateDown();
        void            activateSelectedFile();

        std::size_t                         getFilesAmount() const;

        std::vector<const std::string*>     getVisibleFiles() const;
        std::size_t                         getFirstVisibleFileIndx() const;

        std::size_t                         getRelativeSelectedFileIndx() const;
        std::size_t                         getSelectedFileIndx() const;
        const std::string*                  getSelectedFileName() const;

        int                                 getRelativeActiveFileIndx() const;
        std::size_t                         getActiveFileIndx() const;
        const std::string*                  getActiveFileName() const;
        bool                                isAnyFileActivated() const;



        const std::string*                  getFileNameByIndx(std::size_t indx) const;
        const std::string&                  getCurrentDirectory() const;        

        void                                shuffleFiles();

        void                                openSelectedDirectory();
        void                                openDirectory(const std::string& newDir);
        void                                reloadCurrentDirectory();

        static bool                         isDirectoryExists(const std::string& dir);   
        static void                         createFolder(const std::string& folderPath);
        static std::string                  getFavDirLocation();
        static std::vector<std::string>     getFavDirs();
        static void                         createHardLink(const std::string& old_path, const std::string& new_path);
        static void                         deleteAnyFile(const std::string& filePath);


    private:
        void                                reopenCurrentDir();
        void                                reopen(const std::string& newDir);

    
    
    private:
        std::string                 mCurrentDir;
        std::vector<std::string>    mFiles;
        std::size_t                 mFirstVisibleFileIndx;
        std::size_t                 mSelectedFileIndx;
        const int                   mScreenSizeY;

        bool                        mAnyFileActive;
        std::size_t                 mActiveFileIndx;

        Mode                        mMode;
};

#endif