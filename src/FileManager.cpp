#include "FileManager.hpp"

#include <filesystem>
#include <algorithm>
#include <random>

namespace fs = std::filesystem;

FileManager::FileManager(const std::string& directory, int screenSizeY)
: mFirstVisibleFileIndx(0)
, mSelectedFileIndx(0)
, mScreenSizeY(screenSizeY)
{
    for (const auto& entry : fs::directory_iterator(directory))
    {
        mFiles.push_back(entry.path().filename().string());
        // mFiles.push_back(entry.path().string());
    }
}

const std::string* FileManager::getSelectedFile() const
{
    if (mFiles.empty()) return nullptr;

    return &mFiles[mSelectedFileIndx];
}

const std::string* FileManager::getFileByIndx(std::size_t indx) const
{
    if (indx >= mFiles.size()) return nullptr;

    return &mFiles[indx];
}

std::size_t FileManager::getFirstVisibleFileIndx() const
{
    return mFirstVisibleFileIndx;
}

std::size_t FileManager::getSelectedFileIndx() const
{
    return mSelectedFileIndx;
}

std::size_t FileManager::getRelativeSelectedFileIndx() const
{
    return mSelectedFileIndx - mFirstVisibleFileIndx;
}

std::size_t FileManager::getFilesAmount() const
{
    return mFiles.size();
}

void FileManager::selectUp()
{
    if (mSelectedFileIndx == 0) return;

    mSelectedFileIndx--;
    if (mSelectedFileIndx < mFirstVisibleFileIndx)
    {
        mFirstVisibleFileIndx--;
    }
}

void FileManager::selectDown()
{
    if (mSelectedFileIndx + 1 == mFiles.size()) return;

    mSelectedFileIndx++;
    if (mSelectedFileIndx > mFirstVisibleFileIndx + mScreenSizeY - 2)
    {
        mFirstVisibleFileIndx++;
    }
}

void FileManager::shuffleFiles()
{
    auto rng = std::default_random_engine {};
    std::shuffle(mFiles.begin(), mFiles.end(), rng);
    mSelectedFileIndx = 0;
    mFirstVisibleFileIndx = 0;
}

std::vector<const std::string*> FileManager::getVisibleFiles() const
{
    if (mFiles.empty()) return std::vector<const std::string*>();

    std::vector<const std::string*> visibleFiles;

    int endBound = std::min(mFirstVisibleFileIndx + mScreenSizeY - 1, mFiles.size());
    for (int i = mFirstVisibleFileIndx; i < endBound; ++i)
    {
        visibleFiles.push_back(&mFiles[i]);
    }

    return visibleFiles;
}