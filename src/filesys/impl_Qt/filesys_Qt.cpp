#include "filesys_Qt.h"
#include "uiwrap/string/impl_Qt/stringconvert_Qt.h"
#include "ul/ul.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include "QCoreApplication"
#include "QDir"
#include "QFile"
#include "QStandardPaths"
UL_PRAGMA_WARNINGS_POP
#include <memory>

namespace mb::uiw::implQt {
void CFileSys_Qt::setFileOpErrorStr(const QFile& f, const std::string& op, const std::string& info) const {
    latestError = "file ";
    latestError += op;
    latestError += " failed";
    if (!info.empty())
        latestError += ", " + info;
    latestError += ", details: ";
    latestError += uiw::implQt::qs2s(f.errorString());
    latestError += ", permissions: " + ul::lex_cast<std::string>(f.permissions());
}

bool CFileSys_Qt::saveToTextFile(const std::string& filePathNameExt, const std::string& content) {
    latestError.clear();
    QFile f(s2qs(filePathNameExt));
    if (!f.open(QIODevice::WriteOnly)) {
        setFileOpErrorStr(f, "open");
        return false;
    }
    auto auto_close = ul::finally([&f]() {
        f.close();
    });
    if (f.write(content.c_str()) == -1) {
        setFileOpErrorStr(f, "write");
        return false;
    }
    return true;
}

bool CFileSys_Qt::loadFromTextFile(const std::string& filePathNameExt, std::string& content) const {
    // latestError.clear();
    // QFile f(s2qs(filePathNameExt));
    // if (!f.open(QIODevice::ReadOnly))
    //{
    //    setFileOpErrorStr(f, "open");
    //    return false;
    //}
    // auto auto_close = ul::finally([&f](){ f.close(); });
    // const qint64 size = f.bytesAvailable();
    // std::unique_ptr<char[]> buffer(new char[size]);
    // const qint64 size_read = f.read(buffer.get(), size);
    // if (size_read == -1)
    //{
    //    setFileOpErrorStr(f, "read");
    //    return false;
    //}
    // content.reserve(size_read);
    // content.assign(buffer.get());
    // return true;

    // the implementation above wrote garbage at the end of the output string

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
    if (size == static_cast<decltype(size)>(-1)) {
        this->latestError = "size == -1";
        return false;
    }
    content.resize(static_cast<size_t>(size)); // need the precise size for the string, I guess
    file.seekg(0);
    if (ul::file::fstream_failed(this->latestError, file))
        return false;
    file.read(&content[0], static_cast<std::streamsize>(size));
    return true;
}

bool CFileSys_Qt::copyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) {
    latestError.clear();
    const auto ok = QFile::copy(s2qs(filePathNameExt_From), s2qs(filePathNameExt_To));
    if (!ok) {
        QFile f(s2qs(filePathNameExt_From));
        if (!f.open(QIODevice::ReadOnly)) {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = ul::finally([&f]() {
            f.close();
        });
        setFileOpErrorStr(f, "copy", "target could already be existing, would be overwritten");
    }
    return ok;
}

bool CFileSys_Qt::deleteFile(const std::string& filePathNameExt) {
    latestError.clear();
    const auto ok = QFile::remove(s2qs(filePathNameExt));
    if (!ok) {
        QFile f(s2qs(filePathNameExt));
        if (!f.open(QIODevice::ReadOnly)) {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = ul::finally([&f]() {
            f.close();
        });
        setFileOpErrorStr(f, "remove");
    }
    return ok;
}

bool CFileSys_Qt::renameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) {
    latestError.clear();
    const auto ok = QFile::rename(s2qs(filePathNameExt_From), s2qs(filePathNameExt_To));
    if (!ok) {
        QFile f(s2qs(filePathNameExt_From));
        if (!f.open(QIODevice::ReadOnly)) {
            setFileOpErrorStr(f, "open");
            return false;
        }
        auto auto_close = ul::finally([&f]() {
            f.close();
        });
        setFileOpErrorStr(f, "rename", "target could already be existing, would be overwritten");
    }
    return ok;
}

bool CFileSys_Qt::createFolder(const std::string& folderPath) {
    latestError.clear();
    return QDir().mkpath(s2qs(folderPath));
}

bool CFileSys_Qt::deleteFolder(const std::string& folderPath) {
    latestError.clear();
    return QDir().rmpath(s2qs(folderPath));
}

bool CFileSys_Qt::renameFolder(const std::string& folderPath_From, const std::string& folderPath_To) {
    latestError.clear();
    if (!QDir().rename(s2qs(folderPath_From), s2qs(folderPath_To))) {
        latestError = "folder rename failed, either not existing, target existing, or target opened";
        return false;
    }
    return true;
}

bool CFileSys_Qt::folderExists(const std::string& folderPath) const {
    latestError.clear();
    return QDir().exists(s2qs(folderPath));
}

bool CFileSys_Qt::fileExists(const std::string& filePathNameExt) const {
    latestError.clear();
    return QFile::exists(s2qs(filePathNameExt));
}

bool CFileSys_Qt::isFile(const std::string& filePathNameExt) const {
    latestError.clear();
    QFileInfo fi{s2qs(filePathNameExt)};
    return fi.isFile();
}

std::string CFileSys_Qt::toNativeSeparators(const std::string& path) const {
    latestError.clear();
    return qs2s(QDir::toNativeSeparators(s2qs(path)));
}

std::string CFileSys_Qt::getSystemPath(uiw::file::IFileSys::ESysPathType type, bool withTrailingSeparator) const {
    std::string ret;
    latestError.clear();
    switch (type) {
        case ESysPathType::PROGDATA:
            ret = qs2s(QDir::home().absolutePath());
            break;
        case ESysPathType::ROOT:
            ret = qs2s(QDir::root().absolutePath());
            break;
        case ESysPathType::TEMP:
            ret = qs2s(QDir::temp().absolutePath());
            break;
        case ESysPathType::USER:
            ret = qs2s(QDir::home().absolutePath());
            break;
        case ESysPathType::APPDATA_writable:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
            break;
        case ESysPathType::APPDATA_readonly: {
            const auto locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
            if (locations.count() >= 2)
                ret = qs2s(locations[1]);
            else if (locations.count() == 1)
                ret = qs2s(locations[0]);
            else
                goto default_label;
            break;
        }
        case ESysPathType::DOCUMENTS:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
            break;
        case ESysPathType::MUSIC:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
            break;
        case ESysPathType::PICTURES:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
            break;
        case ESysPathType::MOVIES:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
            break;
        case ESysPathType::DESKTOP:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
            break;
        case ESysPathType::FONTS: {
            const auto locations = QStandardPaths::standardLocations(QStandardPaths::FontsLocation);
            if (!locations.isEmpty())
                ret = qs2s(locations[0]);
            else
                goto default_label;
            break;
        }
        case ESysPathType::CACHE:
            ret = qs2s(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
            break;
        case ESysPathType::CURRENT:
        default:
default_label:
            ret = qs2s(QDir::current().absolutePath());
    }
    if (withTrailingSeparator && !ret.empty())
        ret += FOLDER_SEPARATOR_TO_USE_HERE;
    return ret;
}

std::string CFileSys_Qt::getErrorOfLatestCall() const {
    return this->latestError;
}
} // namespace mb::uiw::implQt
