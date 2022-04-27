#ifndef SUBWINDOW_HPP
#define SUBWINDOW_HPP

#include "Utility.hpp"

#include <vector>

class FileManager;
class MusicPlayer;

class SubWindow
{
    public:
        enum ProgramEvent
        {
            None,
            MusicFinished,
        };

        struct Context
        {
            Context(MusicPlayer& musPlayer, std::vector<ProgramEvent>& progEvents);

            MusicPlayer*                musPlayer;
            std::vector<ProgramEvent>*  mProgEvents;
        };

    public:
        SubWindow(Context context, const std::string& programDir
                    , const Rectangle& bounds);

        virtual void    draw() const = 0;
        virtual void    handleEvent(Event event) = 0;
        virtual void    update() = 0;

        virtual void    select();
        virtual void    deselect();
        virtual bool    iSSelected() const;
    
    protected:
        void                drawRectangle() const;
        const std::string&  getProgramDir() const;
        const Rectangle&    getBounds() const;
        Context             getContext() const;
    
    private:
        const std::string&  mProgramDir;
        const Rectangle     mBounds;
        Context             mContext;
        bool                mSubWindowSelected;
};

#endif