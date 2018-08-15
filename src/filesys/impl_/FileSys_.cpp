// Markus Borris, 2015-18
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#include "FileSys_.h"
#include "toolib/error.h"
#include "toolib/filesys/file.h"
#include "toolib/ignore_arg.h"
#include "toolib/string/lex_cast.h"
#include <cstdio>
#include <fstream>


namespace uiw
{
namespace impl
{
bool CFileSys_::saveToTextFile(const std::string& FilePathNameExt, const std::string& Content)
{
    this->latestError.clear();
    std::ofstream file(FilePathNameExt);
    if (too::file::fstream_failed(this->latestError, file))
        return false;
    file << Content;
    return true;
}

bool CFileSys_::loadFromTextFile(const std::string& filePathNameExt, std::string& content) const
{
    this->latestError.clear();
    std::ifstream file(filePathNameExt);
    if (too::file::fstream_failed(this->latestError, file))
        return false;
    file.seekg(0, std::ios::end);
    if (too::file::fstream_failed(this->latestError, file))
        return false;
    const auto size = file.tellg();
    if (too::file::fstream_failed(this->latestError, file))
        return false;
    if (size == static_cast<decltype(size)>(-1))
    {
        this->latestError = "size == -1";
        return false;
    }
    content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (too::file::fstream_failed(this->latestError, file))
        return false;
    file.read(&content[0], size);
    return true;
}

bool CFileSys_::copyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    this->latestError.clear();
    std::ifstream src(FilePathNameExt_From, std::ios::binary);
    if (too::file::fstream_failed(this->latestError, src))
        return false;
    std::ofstream dst(FilePathNameExt_To, std::ios::binary);
    if (too::file::fstream_failed(this->latestError, dst))
        return false;
    dst << src.rdbuf();
    return true;
}

bool CFileSys_::deleteFile(const std::string& FilePathNameExt)
{
    this->latestError.clear();
    const auto res = remove(FilePathNameExt.c_str());
    if (res)
    {
        this->latestError = too::lex_cast<std::string>(res);
        return false;
    }
    return true;
}

bool CFileSys_::renameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    this->latestError.clear();
    too::ignore_arg(FilePathNameExt_From);
    too::ignore_arg(FilePathNameExt_To);
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::createFolder(const std::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::deleteFolder(const std::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::renameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To)
{
    too::ignore_arg(FolderPath_From);
    too::ignore_arg(FolderPath_To);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::folderExists(const std::string& FolderPath) const
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::fileExists(const std::string& FilePathNameExt) const
{
    this->latestError.clear();
    std::ifstream file(FilePathNameExt, std::ios_base::binary);
    return file ? true : false;
}

bool CFileSys_::isFile(const std::string&) const { throw too::not_implemented{"isFile"}; }

std::string CFileSys_::toNativeSeparators(const std::string& Path) const
{
    too::ignore_arg(Path);
    this->latestError.clear();
    this->latestError = "not implemented";
    return std::string();
}

bool CFileSys_::getSystemPath(IFileSys::ESysPathType Type, std::string& Path, bool WithTrailingSeperator) const
{
    this->latestError.clear();
    switch (Type)
    {
    case ESysPathType::PROGDATA:
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
    this->latestError = "not implemented";
    return false;
}

std::string CFileSys_::getErrorOfLatestCall() const { return this->latestError; }
} // impl
} // uiw
