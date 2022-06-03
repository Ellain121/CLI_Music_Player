#ifndef SUBWINDOW_HPP
#define SUBWINDOW_HPP

#include "Utility.hpp"

#include <vector>

class FileManager;
class MusicPlayer;

class SubWindow
{
    public:
        enum ProgramEventType
        {
            None,
            MusicFinished,
            NewDirectory,
            ReloadFileManagerFiles,
        };

        struct ProgramEvent
        {
            ProgramEvent(ProgramEventType evType, std::string evInfo = "");

            ProgramEventType    eventType;
            std::string         info;
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

        virtual void    drawHelpMenu() const = 0;
        virtual void    draw() const = 0;
        virtual void    handleEvent(Event event) = 0;
        virtual void    update();

        virtual void    select();
        virtual void    deselect();
        virtual bool    iSSelected() const;
    
    protected:
        void                drawRectangle() const;
        const Rectangle&    getBounds() const;
        Context             getContext() const;
    
    private:
        const std::string&  mProgramDir;
        const Rectangle     mBounds;
        Context             mContext;
        bool                mSubWindowSelected;
    
    protected:
        bool                mHelpMenuMode;
};

#endif