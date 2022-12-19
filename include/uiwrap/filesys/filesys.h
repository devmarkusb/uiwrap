// 2015-18

//! \file

#ifndef FILESYS_H_xcsabgzduzbvfget32
#define FILESYS_H_xcsabgzduzbvfget32

#include "uiwrapDEF.h"
#include "../config.h"
#include "ul/ul.h"
#include <stdexcept>
#include <string>


namespace mb::uiw
{
namespace file
{
//####################################################################################################################

class UIWRAPSHARED_EXPORT IFileSys : private ul::NonCopyable
{
public:
    virtual ~IFileSys() = default;

    static IFileSys* getInstance();

    static const char FOLDER_SEPARATOR_TO_USE_HERE = '/';

    virtual bool saveToTextFile(const std::string& filePathNameExt, const std::string& content) = 0;
    virtual bool loadFromTextFile(const std::string& filePathNameExt, std::string& content) const = 0;
    //! Should not overwrite if exists, \returns false then. Call DeleteFile first.
    virtual bool copyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) = 0;
    virtual bool deleteFile(const std::string& filePathNameExt) = 0;
    virtual bool renameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) = 0;
    virtual bool createFolder(const std::string& folderPath) = 0;
    virtual bool deleteFolder(const std::string& folderPath) = 0;
    virtual bool renameFolder(const std::string& folderPath_From, const std::string& folderPath_To) = 0;
    [[nodiscard]] virtual bool folderExists(const std::string& folderPath) const = 0;
    [[nodiscard]] virtual bool fileExists(const std::string& filePathNameExt) const = 0;
    [[nodiscard]] virtual bool isFile(const std::string& filePathNameExt) const = 0;

    [[nodiscard]] virtual std::string toNativeSeparators(const std::string& Path) const = 0;

    //! Explanation and examples.
    /** PROGDATA and USER are the same and yield for Windows something like c:/users/xy,
        which is not the most recommended place in general.
        APPDATA_writable and Co. are much more preferable locations.*/
    enum class ESysPathType
    {
        // PROGRAM,  //! if you miss that - at least Qt config didn't work properly; solution is to parse argv yourself
        USER, //! consider to use it only rarely
        PROGDATA, //! consider to use it only rarely
        TEMP,
        CURRENT,
        ROOT,
        APPDATA_writable, //! e.g. Windows: AppData/Roaming/<APP>
        APPDATA_readonly, //! e.g. Windows: ProgramData/<APP>
        APPCONFIG, //! e.g. Windows: AppData/Local/<APP>
        DOCUMENTS,
        MUSIC,
        PICTURES,
        MOVIES,
        DESKTOP,
        FONTS,
        CACHE,
    };
    [[nodiscard]] virtual std::string getSystemPath(ESysPathType type, bool withTrailingSeparator) const = 0;

    [[nodiscard]] virtual std::string getErrorOfLatestCall() const = 0;
};


//####################################################################################################################

//! Note that the interface forces you to implement std::runtime_error-throwing versions.
//! This is because for load/save detailed error info is absolutely crucial in most cases.
class IFileData
{
public:
    virtual ~IFileData() = default;

    //! Throws std::runtime_error.
    virtual void saveToFile(const std::string& filePathNameExt) const /*noexcept(false)*/ = 0;
    //! Throws std::runtime_error.
    virtual void loadFromFile(const std::string& filePathNameExt) /*noexcept(false)*/ = 0;

    [[nodiscard]] bool saveToFile_(const std::string& filePathNameExt) const
    {
        try
        {
            saveToFile(filePathNameExt);
        }
        catch (const std::runtime_error&)
        {
            return false;
        }
        return true;
    }

    bool loadFromFile_(const std::string& filePathNameExt) noexcept
    {
        try
        {
            loadFromFile(filePathNameExt);
        }
        catch (const std::runtime_error&)
        {
            return false;
        }
        return true;
    }
};
} // namespace file
using IFileSys = file::IFileSys;
using IFileData = file::IFileData;
} // namespace mb::uiw

UIW_HEADER_END

#endif
