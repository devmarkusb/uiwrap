// Markus Borris, 2015-16
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef FILESYS_H_INCL_xcsabgzduzbvfget32
#define FILESYS_H_INCL_xcsabgzduzbvfget32

#include "uiwrapDEF.h"
#include "Toolib/class/non_copyable.h"
#include "Toolib/std/std_extensions.h"
#include <stdexcept>
#include <string>


#pragma push_macro("CopyFile")
#undef CopyFile
#pragma push_macro("DeleteFile")
#undef DeleteFile


namespace uiw
{
namespace file
{
//####################################################################################################################

class UIWRAPSHARED_EXPORT IFileSys : private too::non_copyable
{
public:
    virtual ~IFileSys() = default;

    static IFileSys* GetInstance();

    static const char FOLDER_SEPARATOR_TO_USE_HERE = '/';

    virtual bool SaveToTextFile(const std::string& filePathNameExt, const std::string& content) = 0;
    virtual bool LoadFromTextFile(const std::string& filePathNameExt, std::string& content) = 0;
    //! Should not overwrite if exists, \returns false then. Call DeleteFile first.
    virtual bool CopyFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) = 0;
    virtual bool DeleteFile(const std::string& filePathNameExt) = 0;
    virtual bool RenameFile(const std::string& filePathNameExt_From, const std::string& filePathNameExt_To) = 0;
    virtual bool CreateFolder(const std::string& folderPath) = 0;
    virtual bool DeleteFolder(const std::string& folderPath) = 0;
    virtual bool RenameFolder(const std::string& folderPath_From, const std::string& folderPath_To) = 0;
    virtual bool FolderExists(const std::string& folderPath) = 0;
    virtual bool FileExists(const std::string& filePathNameExt) = 0;

    virtual std::string toNativeSeparators(const std::string& Path) = 0;

    //! Explanation and examples.
    /** PROGDATA and USER are the same and yield for Windows something like c:/users/xy,
        which is not the most recommended place in general.
        APPDATA_writable and Co. are much more preferable locations.*/
    enum class ESysPathType
    {
        PROGRAM, //! path where exe resides
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
    virtual bool GetSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeparator) = 0;

    virtual std::string getErrorOfLatestCall() const = 0;
};


//####################################################################################################################

//! Note that the interface forces you to implement std::runtime_error-throwing versions.
//! This is because for load/save detailed error info is absolutely crucial in most cases.
class IFileData
{
public:
    virtual ~IFileData() = default;

    //! Throws std::runtime_error.
    virtual void saveToFile(const std::string& filePathNameExt) const noexcept(false) = 0;
    //! Throws std::runtime_error.
    virtual void loadFromFile(const std::string& filePathNameExt) noexcept(false) = 0;

    bool saveToFile_(const std::string& filePathNameExt) const
    {
        try { saveToFile(filePathNameExt); }
        catch (const std::runtime_error&) { return false; }
        return true;
    }

    bool loadFromFile_(const std::string& filePathNameExt) noexcept
    {
        try { loadFromFile(filePathNameExt); }
        catch (const std::runtime_error&) { return false; }
        return true;
    }
};

}
using IFileSys  = file::IFileSys;
using IFileData = file::IFileData;
}


#endif
