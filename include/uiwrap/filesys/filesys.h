// Markus Borris, 2015
// This file is part of my uiwrap library. Open source.

//!
/**
*/
//! \file

#ifndef FILESYS_H_INCL_xcsabgzduzbvfget32
#define FILESYS_H_INCL_xcsabgzduzbvfget32

#include <string>
#include "Toolib/tooString.h"
#include "Toolib/class/non_copyable.h"
#include "uiwrapDEF.h"


#pragma push_macro("CopyFile")
#undef CopyFile
#pragma push_macro("DeleteFile")
#undef DeleteFile


namespace uiw
{
namespace file
{
//#############################################################################################################

class UIWRAPSHARED_EXPORT IFileSys : private too::non_copyable
{
public:
    virtual ~IFileSys() = default;

    static IFileSys* GetInstance();

    static const too::charType FOLDER_SEPARATOR_TO_USE_HERE =
        '/'; // _TOOSTR() i.e. u8 doesn't work for char-literal ' ' with mingw yet?!

    virtual bool SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content) = 0;
    virtual bool LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content) = 0;
    //! Should not overwrite if exists, \returns false then. Call DeleteFile first.
    virtual bool CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To) = 0;
    virtual bool DeleteFile(const too::string& FilePathNameExt) = 0;
    virtual bool RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To) = 0;
    virtual bool CreateFolder(const too::string& FolderPath) = 0;
    virtual bool DeleteFolder(const too::string& FolderPath) = 0;
    virtual bool RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To) = 0;
    virtual bool FolderExists(const too::string& FolderPath) = 0;
    virtual bool FileExists(const too::string& FilePathNameExt) = 0;

    virtual too::string toNativeSeparators(const too::string& Path) = 0;

    enum class ESysPathType
    {
        PROGRAM,
        USER,
        PROGDATA,
        TEMP,
        CURRENT,
        ROOT,
    };
    virtual bool GetSystemPath(ESysPathType Type, too::string& Path, bool WithTrailingSeperator) = 0;
};


//#############################################################################################################

class IFileData
{
public:
    virtual ~IFileData() = default;

    virtual bool SaveToFile(const too::string& FilePathNameExt) = 0;
    virtual bool LoadFromFile(const too::string& FilePathNameExt) = 0;
};
}

using IFileSys  = file::IFileSys;
using IFileData = file::IFileData;
}


#endif
