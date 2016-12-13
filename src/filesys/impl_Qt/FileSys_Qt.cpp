// Markus Borris, 2015-16
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#include "FileSys_Qt.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/ignore_arg.h"
#include "Toolib/scope/scopeguard.h"
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

bool CFileSys_Qt::SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content)
{
    QFile f(s2qs(FilePathNameExt));
    if (!f.open(QIODevice::WriteOnly))
        return false;
    too::ScopeGuard guard_close = too::MakeObjGuard(f, &QFile::close);
    too::ignore_arg(guard_close);
    if (f.write(Content.c_str()) == -1)
        return false;
    return true;
}

bool CFileSys_Qt::LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content)
{
    QFile f(s2qs(FilePathNameExt));
    if (!f.open(QIODevice::ReadOnly))
        return false;
    too::ScopeGuard guard_close = too::MakeObjGuard(f, &QFile::close);
    too::ignore_arg(guard_close);
    const qint64 size = f.bytesAvailable();
    std::unique_ptr<char[]> buffer(new char[size]);
    const qint64 size_read = f.read(buffer.get(), size);
    if (size_read == -1)
        return false;
    Content.reserve(size_read);
    Content.assign(buffer.get());
    return true;
}

bool CFileSys_Qt::CopyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    return QFile::copy(s2qs(FilePathNameExt_From), s2qs(FilePathNameExt_To));
}

bool CFileSys_Qt::DeleteFile(const std::string& FilePathNameExt)
{
    return QFile::remove(s2qs(FilePathNameExt));
}

bool CFileSys_Qt::RenameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To)
{
    return QFile::rename(s2qs(FilePathNameExt_From), s2qs(FilePathNameExt_To));
}

bool CFileSys_Qt::CreateFolder(const std::string& FolderPath)
{
    return QDir().mkpath(s2qs(FolderPath));
}

bool CFileSys_Qt::DeleteFolder(const std::string& FolderPath)
{
    return QDir().rmpath(s2qs(FolderPath));
}

bool CFileSys_Qt::RenameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To)
{
    return QDir().rename(s2qs(FolderPath_From), s2qs(FolderPath_To));
}

bool CFileSys_Qt::FolderExists(const std::string& FolderPath)
{
    return QDir().exists(s2qs(FolderPath));
}

bool CFileSys_Qt::FileExists(const std::string& FilePathNameExt)
{
    return QFile::exists(s2qs(FilePathNameExt));
}

std::string CFileSys_Qt::toNativeSeparators(const std::string& Path)
{
    return qs2s(QDir::toNativeSeparators(s2qs(Path)));
}

bool CFileSys_Qt::GetSystemPath(uiw::file::IFileSys::ESysPathType Type, std::string& Path, bool WithTrailingSeparator)
{
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

}
}
