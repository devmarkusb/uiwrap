// Markus Borris, 2015-16
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
    virtual bool SaveToTextFile(const std::string& FilePathNameExt, const std::string& Content) override;
    virtual bool LoadFromTextFile(const std::string& FilePathNameExt, std::string& Content) const override;
    virtual bool CopyFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) override;
    virtual bool DeleteFile(const std::string& FilePathNameExt) override;
    virtual bool RenameFile(const std::string& FilePathNameExt_From, const std::string& FilePathNameExt_To) override;
    virtual bool CreateFolder(const std::string& FolderPath) override;
    virtual bool DeleteFolder(const std::string& FolderPath) override;
    virtual bool RenameFolder(const std::string& FolderPath_From, const std::string& FolderPath_To) override;
    virtual bool FolderExists(const std::string& FolderPath) const override;
    virtual bool FileExists(const std::string& FilePathNameExt) const override;
    virtual bool isFile(const std::string& filePathNameExt) const override;

    virtual std::string toNativeSeparators(const std::string& Path) const override;

    virtual bool GetSystemPath(ESysPathType Type, std::string& Path, bool WithTrailingSeperator) const override;

    virtual std::string getErrorOfLatestCall() const override;

private:
    mutable std::string latestError;
};
}
}

#endif
