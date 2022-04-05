#ifndef SUBWINDOW_HPP
#define SUBWINDOW_HPP

#include "Utility.hpp"

class FileManager;
class MusicPlayer;

class SubWindow
{
    public:
        struct Context
        {
            Context(FileManager& fileManager, MusicPlayer& musPlayer, ProgramData& pData);
            FileManager*    fileManager;
            MusicPlayer*    musPlayer;
            ProgramData*    progData;
        };

    public:
        SubWindow(Context context, const std::string& programDir
                    , const Rectangle& bounds);

        virtual void    draw() const = 0;
        virtual void    handleEvent(Event event) = 0;
        virtual void    update() = 0;
    
    protected:
        const std::string&  getProgramDir() const;
        const Rectangle&    getBounds() const;
        Context             getContext() const;
    
    private:
        const std::string&  mProgramDir;
        const Rectangle     mBounds;
        Context             mContext;
};

#endif