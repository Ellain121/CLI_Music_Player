#include "SubWindow.hpp"

SubWindow::Context::Context(FileManager& fManager, MusicPlayer& mPlayer, ProgramData& pData)
: fileManager(&fManager)
, musPlayer(&mPlayer)
, progData(&pData)
{
}

SubWindow::SubWindow(Context context, const std::string& programDir 
    , const Rectangle& bounds)
: mContext(context)
, mProgramDir(programDir)
, mBounds(bounds)
{
}

const std::string& SubWindow::getProgramDir() const
{
    return mProgramDir;
}

const Rectangle& SubWindow::getBounds() const
{
    return mBounds;
}

SubWindow::Context SubWindow::getContext() const
{
    return mContext;
}