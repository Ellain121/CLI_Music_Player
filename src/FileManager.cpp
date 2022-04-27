#include "FileManager.hpp"

#include <filesystem>
#include <algorithm>
#include <random>

namespace fs = std::filesystem;

namespace
{
    void oneDirUp(std::string& path)
    {
        for (auto itr = path.rbegin(); itr != path.rend(); ++itr)
        {
            if (*itr == '/') break;
            path.pop_back();
        }
        path.pop_back();
    }

    void oneDirDown(std::string& path, const std::string& dirName)
    {
        path += '/' + dirName;
    }
};

FileManager::FileManager(const std::string& directory, int screenSizeY
    , FileManager::Mode mode)
: mCurrentDir(directory)
, mFirstVisibleFileIndx(0)
, mSelectedFileIndx(0)
, mScreenSizeY(screenSizeY)
, mActiveFileIndx(0)
, mAnyFileActive(false)
, mMode(mode)
{
    if (mMode == Mode::FilesManager)
        mFiles.push_back("..");

    for (const auto& entry : fs::directory_iterator(directory))
    {
        mFiles.push_back(entry.path().filename().string());
        // mFiles.push_back(entry.path().string());
    }
}

void FileManager::openSelectedDirectory()
{
    const std::string& selected = mFiles[mSelectedFileIndx];
    if (selected == "..")
    {
        oneDirUp(mCurrentDir);
    }
    else
    {
        std::string newDir = mCurrentDir;
        oneDirDown(newDir, selected);
        if (!fs::is_directory(newDir))
        {
            return;
        }
        mCurrentDir = newDir;
    }
    reopenCurrentDir();
}

void FileManager::openDirectory(const std::string& newDir)
{
    reopen(newDir);
}

void FileManager::reopenCurrentDir()
{
    // throw std::logic_error(mCurrentDir.c_str());

    mFiles.clear();
    mFirstVisibleFileIndx = 0;
    mSelectedFileIndx = 0;
    // activeFile = nullptr;
    // activeFileIndx = 0;

    if (mMode == Mode::FilesManager)
        mFiles.push_back("..");

    for (const auto& entry : fs::directory_iterator(mCurrentDir))
    {
        mFiles.push_back(entry.path().filename().string());
        // mFiles.push_back(entry.path().string());
    }
}

void FileManager::reopen(const std::string& newDir)
{
    mCurrentDir = newDir;
    reopenCurrentDir();
}

const std::string& FileManager::getCurrentDirectory() const
{
    return mCurrentDir;
}

const std::string* FileManager::getSelectedFileName() const
{
    if (mFiles.empty()) return nullptr;

    return &mFiles[mSelectedFileIndx];
}

const std::string* FileManager::getActiveFileName() const
{
    if (!mAnyFileActive) return nullptr;

    return &mFiles[mActiveFileIndx];
}

const std::string* FileManager::getFileNameByIndx(std::size_t indx) const
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

std::size_t FileManager::getActiveFileIndx() const
{
    return mActiveFileIndx;
}

std::size_t FileManager::getRelativeSelectedFileIndx() const
{
    return mSelectedFileIndx - mFirstVisibleFileIndx;
}

int FileManager::getRelativeActiveFileIndx() const
{
    return mActiveFileIndx - mFirstVisibleFileIndx;
}

std::size_t FileManager::getFilesAmount() const
{
    return mFiles.size();
}

void FileManager::activateSelectedFile()
{
    mAnyFileActive = true;
    mActiveFileIndx = mSelectedFileIndx;
}

bool FileManager::isAnyFileActivated() const
{
    return mAnyFileActive;
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

void FileManager::activateUp()
{
    if (mActiveFileIndx == 0)
    {
        mActiveFileIndx = mFiles.size() - 1;
        return;
    }

    mActiveFileIndx--;
}

void FileManager::activateDown()
{
    if (mActiveFileIndx == mFiles.size() - 1)
    {
        mActiveFileIndx = 0;
        return;
    }

    mActiveFileIndx++;
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