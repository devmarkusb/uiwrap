#include "filesys_.h"
#include "toolib/filesys/file.h"
#include "toolib/string/lex_cast.h"
#include "ul/ul.h"
#include <cstdio>
#include <filesystem>
#include <fstream>

namespace mb::uiw::impl
{
bool CFileSys_::saveToTextFile(const std::string& filePathNameExt, const std::string& Content)
{
    this->latestError.clear();
    std::ofstream file(filePathNameExt);
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    file << Content;
    return true;
}

bool CFileSys_::loadFromTextFile(const std::string& filePathNameExt, std::string& content) const
{
    this->latestError.clear();
    std::ifstream file(filePathNameExt);
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    file.seekg(0, std::ios::end);
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    const auto size = file.tellg();
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    if (size == static_cast<decltype(size)>(-1))
    {
        this->latestError = "size == -1";
        return false;
    }
    content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    file.read(&content[0], size);
    return true;
}

bool CFileSys_::copyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To)
{
    this->latestError.clear();
    std::ifstream src(filePathNameExt_From, std::ios::binary);
    if (ul::file::fstream_failed(this->latestError, src))
        return false;
    std::ofstream dst(filePathNameExt_To, std::ios::binary);
    if (ul::file::fstream_failed(this->latestError, dst))
        return false;
    dst << src.rdbuf();
    return true;
}

bool CFileSys_::deleteFile(const std::string& filePathNameExt)
{
    this->latestError.clear();
    const auto res = remove(filePathNameExt.c_str());
    if (res)
    {
        this->latestError = ul::lex_cast<std::string>(res);
        return false;
    }
    return true;
}

bool CFileSys_::renameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To)
{
    this->latestError.clear();
    ul::ignore_unused(filePathNameExt_From);
    ul::ignore_unused(filePathNameExt_To);
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::createFolder(const std::string& folderPath)
{
    ul::ignore_unused(folderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::deleteFolder(const std::string& folderPath)
{
    ul::ignore_unused(folderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::renameFolder(const std::string& folderPath_From, const std::string& folderPath_To)
{
    ul::ignore_unused(folderPath_From);
    ul::ignore_unused(folderPath_To);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::folderExists(const std::string& folderPath) const
{
    ul::ignore_unused(folderPath);
    this->latestError.clear();
    this->latestError = "not implemented";
    return false;
}

bool CFileSys_::fileExists(const std::string& filePathNameExt) const
{
    this->latestError.clear();
    std::ifstream file(filePathNameExt, std::ios_base::binary);
    return file ? true : false;
}

bool CFileSys_::isFile(const std::string&) const
{
    throw ul::not_implemented{"isFile"};
}

std::string CFileSys_::toNativeSeparators(const std::string& Path) const
{
    ul::ignore_unused(Path);
    this->latestError.clear();
    this->latestError = "not implemented";
    return {};
}

std::string CFileSys_::getSystemPath(uiw::file::IFileSys::ESysPathType type, bool withTrailingSeparator) const
{
    this->latestError.clear();
    std::string ret;
    switch (type)
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
            break;
        case ESysPathType::APPDATA_writable:
            ret = std::filesystem::temp_directory_path();
            break;
        default:
            break;
    }
    this->latestError = "not implemented";
    if (withTrailingSeparator)
        return ret + FOLDER_SEPARATOR_TO_USE_HERE;
    else
        return {};
}

std::string CFileSys_::getErrorOfLatestCall() const
{
    return this->latestError;
}
} // namespace mb::uiw::impl
