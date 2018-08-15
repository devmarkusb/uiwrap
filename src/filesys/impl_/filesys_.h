// Markus Borris, 2015-18
// This file is part of my uiwrap library.

//!
/**
*/
//! \file

#ifndef FILESYS__H_dklzuix3xh8734xr387x2xt35g269x5tgf
#define FILESYS__H_dklzuix3xh8734xr387x2xt35g269x5tgf

#include "uiwrap/filesys/filesys.h"
#include "uiwrapDEF.h"
#include <string>


#if TOO_OS_WINDOWS == 1
#undef CopyFile
#undef DeleteFile
#endif


namespace uiw
{
namespace impl
{
class UIWRAPSHARED_EXPORT CFileSys_ : public uiw::IFileSys
{
public:
    bool saveToTextFile(const std::string& FilePathNameExt, const std::string& Content) override;
    bool loadFromTextFile(const std::string& FilePathNameExt, std::string& Content) const override;
    bool copyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) override;
    bool deleteFile(const std::string& FilePathNameExt) override;
    bool renameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) override;
    bool createFolder(const std::string& FolderPath) override;
    bool deleteFolder(const std::string& FolderPath) override;
    bool renameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To) override;
    bool folderExists(const std::string& FolderPath) const override;
    bool fileExists(const std::string& FilePathNameExt) const override;
    bool isFile(const std::string& filePathNameExt) const override;

    std::string toNativeSeparators(const std::string& Path) const override;

    bool getSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeperator) const override;

    std::string getErrorOfLatestCall() const override;

private:
#include "toolib/PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"
    mutable std::string latestError;
#include "toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"
};
} // impl
} // uiw

#endif
