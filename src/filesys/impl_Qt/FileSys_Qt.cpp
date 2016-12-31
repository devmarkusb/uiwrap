// Markus Borris, 2015-16
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#include "FileSys_Qt.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/ignore_arg.h"
#include "Toolib/finally.h"
#include "Toolib/string/lex_cast.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <memory>


namespace uiw
{
namespace implQt
{

void CFileSys_Qt::setFileOpErrorStr(const QFile& f, const std::string& op, const std::string& info) const
{
    latestError = "file ";
    latestError+= op;
    latestError+= " failed";
    if (!info.empty())
        latestError+= ", " + info;
    latestError+= ", details: ";
    latestError+= uiw::implQt::qs2s(f.errorString());
    latestError+= ", permissions: " + too::lex_cast<std::string>(f.permissions());
}

bool CFileSys_Qt::SaveToTextFile(const std::string& filePathNameExt, const std::string& content)
{
    latestError.clear();
    QFile f(s2qs(filePathNameExt));
    if (!f.open(QIODevice::WriteOnly))
    {
        setFileOpErrorStr(f, "open");
        return false;
    }
    auto auto_close = too::finally([&f](){ f.close(); });
    if (f.write(content.c_str()) == -1)
    {
        setFileOpErrorStr(f, "write");
        return false;
    }
    return true;
}

bool CFileSys_Qt::LoadFromTextFile(const std::string& filePathNameExt, std::string& content) const
{
    latestError.clear();
    QFile f(s2qs(filePathNameExt));
    if (!f.open(QIODevice::ReadOnly))
    {
        setFileOpErrorStr(f, "open");
        return false;
    }
    auto auto_close = too::finally([&f](){ f.close(); });
    const qint64 size = f.bytesAvailable();
    std::unique_ptr<char[]> buffer(new char[size]);
    const qint64 size_read = f.read(buffer.get(), size);
    if (size_read == -1)
    {
        setFileOpErrorStr(f, "read");
        return false;
    }
    content.reserve(size_read);
    content.assign(buffer.get());
    return true;
}

bool CFileSys_Qt::CopyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To)
{
    latestError.clear();
    const auto ok = QFile::copy(s2qs(filePathNameExt_From), s2qs(filePathNameExt_To));
    if (!ok)
    {
        QFile f(s2qs(filePathNameExt_From));
        if (!f.open(QIODevice::ReadOnly))
        {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = too::finally([&f](){ f.close(); });
        setFileOpErrorStr(f, "copy", "target could already be existing, would be overwritten");
    }
    return ok;
}

bool CFileSys_Qt::DeleteFile(const std::string& filePathNameExt)
{
    latestError.clear();
    const auto ok = QFile::remove(s2qs(filePathNameExt));
    if (!ok)
    {
        QFile f(s2qs(filePathNameExt));
        if (!f.open(QIODevice::ReadOnly))
        {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = too::finally([&f](){ f.close(); });
        setFileOpErrorStr(f, "remove");
    }
    return ok;
}

bool CFileSys_Qt::RenameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To)
{
    latestError.clear();
    const auto ok = QFile::rename(s2qs(filePathNameExt_From), s2qs(filePathNameExt_To));
    if (!ok)
    {
        QFile f(s2qs(filePathNameExt_From));
        if (!f.open(QIODevice::ReadOnly))
        {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = too::finally([&f](){ f.close(); });
        setFileOpErrorStr(f, "rename", "target could already be existing, would be overwritten");
    }
    return ok;
}

bool CFileSys_Qt::CreateFolder(const std::string& folderPath)
{
    latestError.clear();
    return QDir().mkpath(s2qs(folderPath));
}

bool CFileSys_Qt::DeleteFolder(const std::string& folderPath)
{
    latestError.clear();
    return QDir().rmpath(s2qs(folderPath));
}

bool CFileSys_Qt::RenameFolder(const std::string& folderPath_From, const std::string& folderPath_To)
{
    latestError.clear();
    if (!QDir().rename(s2qs(folderPath_From), s2qs(folderPath_To)))
    {
        latestError = "folder rename failed, either not existing, target existing, or target opened";
        return false;
    }
    return true;
}

bool CFileSys_Qt::FolderExists(const std::string& folderPath) const
{
    latestError.clear();
    return QDir().exists(s2qs(folderPath));
}

bool CFileSys_Qt::FileExists(const std::string& filePathNameExt) const
{
    latestError.clear();
    return QFile::exists(s2qs(filePathNameExt));
}

std::string CFileSys_Qt::toNativeSeparators(const std::string& Path) const
{
    latestError.clear();
    return qs2s(QDir::toNativeSeparators(s2qs(Path)));
}

bool CFileSys_Qt::GetSystemPath(uiw::file::IFileSys::ESysPathType Type, std::string& Path, bool WithTrailingSeparator) const
{
    latestError.clear();
    switch (Type)
    {
    case ESysPathType::PROGDATA:
        Path = qs2s(QDir::home().absolutePath());
        break;
    case ESysPathType::PROGRAM:
        Path = qs2s(QDir(QCoreApplication::applicationDirPath()).absolutePath());
        break;
    case ESysPathType::ROOT:
        Path = qs2s(QDir::root().absolutePath());
        break;
    case ESysPathType::TEMP:
        Path = qs2s(QDir::temp().absolutePath());
        break;
    case ESysPathType::USER:
        Path = qs2s(QDir::home().absolutePath());
        break;
    case ESysPathType::APPDATA_writable:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        break;
    case ESysPathType::APPDATA_readonly:
    {
        const auto locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        if (locations.count() >= 2)
            Path = qs2s(locations[1]);
        else if (locations.count() == 1)
            Path = qs2s(locations[0]);
        else
            goto default_label;
        break;
    }
    case ESysPathType::DOCUMENTS:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        break;
    case ESysPathType::MUSIC:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
        break;
    case ESysPathType::PICTURES:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        break;
    case ESysPathType::MOVIES:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
        break;
    case ESysPathType::DESKTOP:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        break;
    case ESysPathType::FONTS:
    {
        const auto locations = QStandardPaths::standardLocations(QStandardPaths::FontsLocation);
        if (!locations.isEmpty())
            Path = qs2s(locations[0]);
        else
            goto default_label;
        break;
    }
    case ESysPathType::CACHE:
        Path = qs2s(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        break;
    case ESysPathType::CURRENT:
    default:
    default_label:
        Path = qs2s(QDir::current().absolutePath());
    }
    if (WithTrailingSeparator && !Path.empty())
        Path += FOLDER_SEPARATOR_TO_USE_HERE;
    return true;
}

std::string CFileSys_Qt::getErrorOfLatestCall() const
{
    return this->latestError;
}

}
}
