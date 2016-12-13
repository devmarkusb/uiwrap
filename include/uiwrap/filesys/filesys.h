// Markus Borris, 2015
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef FILESYS_H_INCL_xcsabgzduzbvfget32
#define FILESYS_H_INCL_xcsabgzduzbvfget32

#include "uiwrapDEF.h"
#include "Toolib/class/non_copyable.h"
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

    virtual bool SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content) = 0;
    virtual bool LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content) = 0;
    //! Should not overwrite if exists, \returns false then. Call DeleteFile first.
    virtual bool CopyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) = 0;
    virtual bool DeleteFile(const std::string& FilePathNameExt) = 0;
    virtual bool RenameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) = 0;
    virtual bool CreateFolder(const std::string& FolderPath) = 0;
    virtual bool DeleteFolder(const std::string& FolderPath) = 0;
    virtual bool RenameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To) = 0;
    virtual bool FolderExists(const std::string& FolderPath) = 0;
    virtual bool FileExists(const std::string& FilePathNameExt) = 0;

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
};


//####################################################################################################################

class IFileData
{
public:
    virtual ~IFileData() = default;

    virtual bool SaveToFile(const std::string& FilePathNameExt) const = 0;
    virtual bool LoadFromFile(const std::string& FilePathNameExt) = 0;
};

}
using IFileSys  = file::IFileSys;
using IFileData = file::IFileData;
}


#endif
