// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#include "FileSys_.h"
#include <fstream>
#include <cstdio>
#include "Toolib/argsused.h"


namespace uiw
{
namespace impl
{

bool CFileSys_::SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content)
{
    std::ofstream file(FilePathNameExt);
    if (!file)
        return false;
    file << Content;
    return true;
}

bool CFileSys_::LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content)
{
    std::ifstream file(FilePathNameExt);
    if (!file)
        return false;
    file.seekg(0, std::ios::end);
    if (!file)
        return false;
    const auto size = file.tellg();
    if (!file || size == static_cast<decltype(size)>(-1))
        return false;
    Content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (!file)
        return false;
    file.read(&Content[0], size);
    return true;
}

bool CFileSys_::CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    std::ifstream src(FilePathNameExt_From, std::ios::binary);
    if (!src)
        return false;
    std::ofstream dst(FilePathNameExt_To, std::ios::binary);
    if (!dst)
        return false;
    dst << src.rdbuf();
    return true;
}

bool CFileSys_::DeleteFile(const too::string& FilePathNameExt)
{
    return !std::remove(FilePathNameExt.c_str());
}

bool CFileSys_::RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    too::ignore_arg(FilePathNameExt_From);
    too::ignore_arg(FilePathNameExt_To);
    return false;
}

bool CFileSys_::CreateFolder(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_::DeleteFolder(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_::RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To)
{
    too::ignore_arg(FolderPath_From);
    too::ignore_arg(FolderPath_To);
    return false;
}

bool CFileSys_::FolderExists(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_::FileExists(const too::string& FilePathNameExt)
{
    std::ifstream file(FilePathNameExt, std::ios_base::binary);
    return file ? true : false;
}

too::string CFileSys_::toNativeSeparators(const too::string& Path)
{
    too::ignore_arg(Path);
    return too::string();
}

bool CFileSys_::GetSystemPath(IFileSys::ESysPathType Type, too::string& Path, bool WithTrailingSeperator)
{
    switch (Type)
    {
    case ESysPathType::PROGDATA:
        break;
    case ESysPathType::PROGRAM:
        break;
    case ESysPathType::ROOT:
        break;
    case ESysPathType::TEMP:
        break;
    case ESysPathType::USER:
        break;
    case ESysPathType::CURRENT:
    default:
        break;
    }
    if (WithTrailingSeperator)
        Path += FOLDER_SEPARATOR_TO_USE_HERE;
    return false;
}
}
}
