// Markus Borris, 2015-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#include "FileSys_.h"
#include <fstream>
#include <cstdio>
#include "Toolib/ignore_arg.h"
#include "Toolib/string/lex_cast.h"


namespace uiw
{
namespace impl
{

template <class FStream>
bool CFileSys_::fstream_failed(const FStream& fs)
{
    if (fs)
        return false;
    if (fs.eof())
        this->latestError = "eof";
    else if (fs.bad())
        this->latestError = "bad";
    else if (fs.fail())
        this->latestError = "fail";
    return true;
}

bool CFileSys_::SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content)
{
    this->latestError.clear();
    std::ofstream file(FilePathNameExt);
    if (fstream_failed(file))
        return false;
    file << Content;
    return true;
}

bool CFileSys_::LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content)
{
    this->latestError.clear();
    std::ifstream file(FilePathNameExt);
    if (fstream_failed(file))
        return false;
    file.seekg(0, std::ios::end);
    if (fstream_failed(file))
        return false;
    const auto size = file.tellg();
    if (fstream_failed(file))
        return false;
    if (size == static_cast<decltype(size)>(-1))
    {
        this->latestError = "size == -1";
        return false;
    }
    Content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (fstream_failed(file))
        return false;
    file.read(&Content[0], size);
    return true;
}

bool CFileSys_::CopyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    this->latestError.clear();
    std::ifstream src(FilePathNameExt_From, std::ios::binary);
    if (fstream_failed(src))
        return false;
    std::ofstream dst(FilePathNameExt_To, std::ios::binary);
    if (fstream_failed(dst))
        return false;
    dst << src.rdbuf();
    return true;
}

bool CFileSys_::DeleteFile(const std::string& FilePathNameExt)
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

bool CFileSys_::RenameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    this->latestError.clear();
    too::ignore_arg(FilePathNameExt_From);
    too::ignore_arg(FilePathNameExt_To);
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::CreateFolder(const std::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::DeleteFolder(const std::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::RenameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To)
{
    too::ignore_arg(FolderPath_From);
    too::ignore_arg(FolderPath_To);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::FolderExists(const std::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::FileExists(const std::string& FilePathNameExt)
{
    this->latestError.clear();
    std::ifstream file(FilePathNameExt, std::ios_base::binary);
    return file ? true : false;
}

std::string CFileSys_::toNativeSeparators(const std::string& Path)
{
    too::ignore_arg(Path);
    this->latestError.clear();
    this->latestError = "not implemented";
    return std::string();
}

bool CFileSys_::GetSystemPath(IFileSys::ESysPathType Type, std::string& Path, bool WithTrailingSeperator)
{
    this->latestError.clear();
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
    this->latestError = "not implemented";
    return false;
}

std::string CFileSys_::getErrorOfLatestCall() const
{
    return this->latestError;
}
} // impl
} // uiw
